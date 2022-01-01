#include <semaphore.h>

#include "tools.h"
#include <atomic>
#include <iostream>
#include <linux/futex.h>
#include <syscall.h>
#include <thread>

static constexpr int kLoop = 1000;

/**
 * Benchmark overhead and latency for waking another thread.
 * sem_t, futex, todo: pipe, condvar
 *
 * 1. overhead for waker
 * 2. latency of wakeup
 *
 */

static void benchmark_sem(int core1, int core2) {
  std::atomic<int> cnt{0};
  sem_t sem;
  sem_init(&sem, 0, 0);

  auto th1 = std::thread([&]() {
    affinity_set(core1);

    uint64_t total_overhead = 0;
    uint64_t total_latency = 0;

    for (int i = 1; i <= kLoop; i++) {
      while (cnt.load() != i) {
        asm("pause");
      }
      // make sure thread 2 not spin in userspace
      usleep(10);
      uint64_t begin = clock_ns();
      sem_post(&sem);
      uint64_t overhead = duration_ns(begin);
      while (cnt.load() == i) {
        asm("pause");
      };
      uint64_t latency = duration_ns(begin);

      total_overhead += overhead;
      total_latency += latency;
    }

    uint64_t avg_overhead = total_overhead / kLoop;
    uint64_t avg_latency = total_latency / kLoop;

    std::cout << "Wake with sem, avg overhead " << avg_overhead << "ns, "
              << "avg latency " << avg_latency << "ns." << std::endl;
  });

  auto th2 = std::thread([&]() {
    affinity_set(core2);

    for (int i = 1; i <= kLoop; i++) {
      cnt.store(i);
      sem_wait(&sem);
    }
    cnt.store(kLoop + 1);
  });

  th1.join();
  th2.join();
}

#define futex(uaddr, op, val, timeout, uaddr2, val3, opflags)                  \
  syscall(SYS_futex, uaddr, op | opflags, val, timeout, uaddr2, val3)

static inline int futex_wait(u_int32_t *uaddr, u_int32_t val,
                             struct timespec *timeout, int opflags) {
  return futex(uaddr, FUTEX_WAIT, val, timeout, NULL, 0, opflags);
}

static inline int futex_wake(u_int32_t *uaddr, int nr_wake, int opflags) {
  return futex(uaddr, FUTEX_WAKE, nr_wake, NULL, NULL, 0, opflags);
}

static void benchmark_futex(int core1, int core2) {
  std::atomic<int> cnt{0};
  uint32_t futex = 0;

  uint64_t begin = clock_ns();
  for (int i = 1; i <= kLoop; i++) {
    futex_wake(&futex, 1, FUTEX_PRIVATE_FLAG);
  }
  uint64_t duration = duration_ns(begin);
  std::cout << "Call futex_wake, avg overhead " << duration / kLoop << "ns"
            << std::endl;

  auto th1 = std::thread([&]() {
    affinity_set(core1);

    uint64_t total_overhead = 0;
    uint64_t total_latency = 0;

    for (int i = 1; i <= kLoop; i++) {
      while (cnt.load() != i) {
        asm("pause");
      }
      uint64_t begin = clock_ns();
      int wake = futex_wake(&futex, 1, FUTEX_PRIVATE_FLAG);
      while (!wake) {
        begin = clock_ns();
        wake = futex_wake(&futex, 1, FUTEX_PRIVATE_FLAG);
      }
      uint64_t overhead = duration_ns(begin);
      while (cnt.load() == i) {
        asm("pause");
      };
      uint64_t latency = duration_ns(begin);

      total_overhead += overhead;
      total_latency += latency;
    }

    uint64_t avg_overhead = total_overhead / kLoop;
    uint64_t avg_latency = total_latency / kLoop;

    std::cout << "Wake with futex, avg overhead " << avg_overhead << "ns, "
              << "avg latency " << avg_latency << "ns." << std::endl;
  });

  auto th2 = std::thread([&]() {
    affinity_set(core2);

    for (int i = 1; i <= kLoop; i++) {
      cnt.store(i);
      futex_wait(&futex, 0, NULL, FUTEX_PRIVATE_FLAG);
    }
    cnt.store(kLoop + 1);
  });

  th1.join();
  th2.join();
}

static void benchmark_pipe(int core1, int core2) {}

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cout << "Usage: %s <core1> <core2>" << std::endl;
    return -1;
  }

  int core1 = std::stoi(argv[1]);
  int core2 = std::stoi(argv[2]);

  benchmark_sem(core1, core2);
  benchmark_futex(core1, core2);

  return 0;
}