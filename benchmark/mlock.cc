#include "tools.h"

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/mman.h>

static struct ibv_device *ib_dev = NULL;
static struct ibv_context *ib_ctx;
static struct ibv_pd *ib_pd = NULL;
static bool odp = false;

static constexpr size_t kLoop = 1000;
static constexpr size_t kMaxSize = 32 * 1024ULL * 1024ULL;

static void benchmark(char *ptr, size_t len, int loop) {
  uint64_t total = 0;
  for (int i = 0; i < loop; i++) {
    auto begin = clock_ns();
    int ret = mlock(ptr, len);
    if (ret != 0) {
      std::cerr << "Failed to mlock, ret " << errno << ", check ulimit please."
                << std::endl;
      abort();
    }
    ret = munlock(ptr, len);
    if (ret != 0) {
      std::cerr << "Failed to munlock, ret " << errno << std::endl;
      abort();
    }
    ptr += len;
    auto end = clock_ns();
    total += end - begin;
  }

  std::cout << format_size(len) << " \t\t"
            << format_latency(total / (double)loop) << std::endl;
}

int main() {
  for (size_t mem = 128; mem <= kMaxSize; mem *= 2) {
    char *ptr = (char *)malloc(mem * kLoop);
    if (ptr == nullptr)
      break;
    benchmark(ptr, mem, kLoop);
    free(ptr);
  }
  std::cout << std::endl;

  std::cout << "zero memory before lock" << std::endl;
  for (size_t mem = 128; mem <= kMaxSize; mem *= 2) {
    char *ptr = (char *)malloc(mem * kLoop);
    if (ptr == nullptr)
      break;
    memset(ptr, 0, mem);
    benchmark(ptr, mem, kLoop);
    free(ptr);
  }

  return 0;
}