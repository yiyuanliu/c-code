#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <x86intrin.h>

/*
 * Need to compile with --std=gnu99
 *
 * Refer to this link for more accurate data
 * http://btorpey.github.io/blog/2014/02/18/clock-sources-in-linux/
 *
 * How to determine tsc frequency?
 * https://github.com/trailofbits/tsc_freq_khz
 */

static inline struct timeval timeofday() {
  struct timeval time;
  gettimeofday(&time, NULL);

  return time;
}

static inline struct timespec time_real() {
  struct timespec time;
  clock_gettime(CLOCK_REALTIME, &time);

  return time;
}

static inline struct timespec time_mono() {
  struct timespec time;
  clock_gettime(CLOCK_MONOTONIC, &time);

  return time;
}

static inline long long time_rdtsc_intrin() { return __rdtsc(); }

static inline long long time_rdtscp_intrin() {
  unsigned int tmp;
  return __rdtscp(&tmp);
}

static inline long long time_rdtsc_asm() {
  unsigned int lo, hi;
  long long time;
  asm volatile("rdtsc"
               : "=a"(lo), "=d"(hi) /* outputs */
               : "a"(0)             /* inputs */
               : "%ebx", "%ecx");
  time = hi;
  time = time << 32 | lo;

  return time;
}

static inline long long time_cpuid_rdtsc_asm() {
  unsigned int lo, hi;
  long long time;
  asm volatile("cpuid \n"
               "rdtsc"
               : "=a"(lo), "=d"(hi) /* outputs */
               : "a"(0)             /* inputs */
               : "%ebx", "%ecx");
  time = hi;
  time = (time << 32) | lo;

  return time;
}

static inline long long time_rdtscp_asm() {
  unsigned int lo, hi;
  long long time;
  asm volatile("rdtscp"
               : "=a"(lo), "=d"(hi) /* outputs */
               : "a"(0)             /* inputs */
               : "%ebx", "%ecx");
  time = hi;
  time = (time << 32) | lo;

  return time;
}

static inline long long time_rdtscp_cpuid_asm() {
  unsigned int lo, hi;
  long long time;
  asm volatile("rdtscp" : "=a"(lo), "=d"(hi) : "a"(0) : "%ebx", "%ecx");
  asm volatile("cpuid");
  time = hi;
  time = (time << 32) | lo;

  return time;
}

#define N 1000000

int main() {
  struct timespec res;
  clock_getres(CLOCK_REALTIME, &res);
  printf("Real time res: %ldns\n", res.tv_nsec);
  clock_getres(CLOCK_MONOTONIC, &res);
  printf("Mono time res: %ldns\n", res.tv_nsec);

  struct timespec begin, end;
  long long nanosec;

  clock_gettime(CLOCK_REALTIME, &begin);
  for (int i = 0; i < N; i++) {
    timeofday();
  }
  clock_gettime(CLOCK_REALTIME, &end);
  nanosec =
      (end.tv_sec - begin.tv_sec) * 1000000000 + (end.tv_nsec - begin.tv_nsec);
  printf("timeofday: %lldns\n", nanosec / N);

  clock_gettime(CLOCK_REALTIME, &begin);
  for (int i = 0; i < N; i++) {
    time_real();
  }
  clock_gettime(CLOCK_REALTIME, &end);
  nanosec =
      (end.tv_sec - begin.tv_sec) * 1000000000 + (end.tv_nsec - begin.tv_nsec);
  printf("time_real: %lldns\n", nanosec / N);

  clock_gettime(CLOCK_REALTIME, &begin);
  for (int i = 0; i < N; i++) {
    time_mono();
  }
  clock_gettime(CLOCK_REALTIME, &end);
  nanosec =
      (end.tv_sec - begin.tv_sec) * 1000000000 + (end.tv_nsec - begin.tv_nsec);
  printf("time_mono: %lldns\n", nanosec / N);

  clock_gettime(CLOCK_REALTIME, &begin);
  for (int i = 0; i < N; i++) {
    time_rdtsc_intrin();
  }
  clock_gettime(CLOCK_REALTIME, &end);
  nanosec =
      (end.tv_sec - begin.tv_sec) * 1000000000 + (end.tv_nsec - begin.tv_nsec);
  printf("time_rdtsc_intrin: %lldns\n", nanosec / N);

  clock_gettime(CLOCK_REALTIME, &begin);
  for (int i = 0; i < N; i++) {
    time_rdtscp_intrin();
  }
  clock_gettime(CLOCK_REALTIME, &end);
  nanosec =
      (end.tv_sec - begin.tv_sec) * 1000000000 + (end.tv_nsec - begin.tv_nsec);
  printf("time_rdtscp_intrin: %lldns\n", nanosec / N);

  clock_gettime(CLOCK_REALTIME, &begin);
  for (int i = 0; i < N; i++) {
    time_rdtsc_asm();
  }
  clock_gettime(CLOCK_REALTIME, &end);
  nanosec =
      (end.tv_sec - begin.tv_sec) * 1000000000 + (end.tv_nsec - begin.tv_nsec);
  printf("time_rdtsc_asm: %lldns\n", nanosec / N);

  clock_gettime(CLOCK_REALTIME, &begin);
  for (int i = 0; i < N; i++) {
    time_cpuid_rdtsc_asm();
  }
  clock_gettime(CLOCK_REALTIME, &end);
  nanosec =
      (end.tv_sec - begin.tv_sec) * 1000000000 + (end.tv_nsec - begin.tv_nsec);
  printf("time_cpuid_rdtsc_asm: %lldns\n", nanosec / N);

  clock_gettime(CLOCK_REALTIME, &begin);
  for (int i = 0; i < N; i++) {
    time_rdtscp_asm();
  }
  clock_gettime(CLOCK_REALTIME, &end);
  nanosec =
      (end.tv_sec - begin.tv_sec) * 1000000000 + (end.tv_nsec - begin.tv_nsec);
  printf("time_rdtscp_asm: %lldns\n", nanosec / N);

  clock_gettime(CLOCK_REALTIME, &begin);
  for (int i = 0; i < N; i++) {
    time_rdtscp_cpuid_asm();
  }
  clock_gettime(CLOCK_REALTIME, &end);
  nanosec =
      (end.tv_sec - begin.tv_sec) * 1000000000 + (end.tv_nsec - begin.tv_nsec);
  printf("time_rdtscp_cpuid_asm: %lldns\n", nanosec / N);

  long long total_circle = 0;
  for (int i = 0; i < N; i++) {
    long long begin, end;
    begin = time_cpuid_rdtsc_asm();
    end = time_rdtscp_asm();

    if (end < begin) {
      printf("err: end < begin\n");
      return -1;
    }

    total_circle += end - begin;
  }
  printf("cpuid+rdtsc & rdtscp+cpuid pair: %lldcycle\n", total_circle / N);

  return 0;
}
