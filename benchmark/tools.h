#pragma once

#include <cstdlib>
#include <pthread.h>
#include <sstream>
#include <stdint.h>
#include <string>
#include <time.h>
#include <unistd.h>

static inline uint64_t clock_us() {
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC_RAW, &now);

  return now.tv_sec * 1000000 + now.tv_nsec / 1000;
}

static inline uint64_t duration_us(uint64_t begin) {
  return clock_us() - begin;
}

static inline uint64_t clock_ns() {
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC_RAW, &now);

  return now.tv_sec * 1000000000 + now.tv_nsec;
}

static inline uint64_t duration_ns(uint64_t begin) {
  return clock_ns() - begin;
}

static int affinity_set(int core) {
  cpu_set_t cpu_set;
  CPU_ZERO(&cpu_set);
  CPU_SET(core, &cpu_set);

  return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set), &cpu_set);
}

static std::string format_size(double bytes) {
  int n = 0;
  while (bytes >= 1024 && n < 4) {
    bytes = bytes / 1024;
    n++;
  }
  std::string unit;
  switch (n) {
    case 0: unit = "B"; break;
    case 1: unit = "KB"; break;
    case 2: unit = "MB"; break;
    case 3: unit = "GB"; break;
    case 4:
    default: unit = "TB";
  }

  std::stringstream ss;
  ss.precision(4);
  ss << bytes << unit;
  return ss.str();
}

static std::string format_latency(double ns) {
  int n = 0;
  while (ns > 1000 && n < 3) {
    ns = ns / 1000;
    n++;
  }
  std::string unit;
  switch (n) {
    case 0: unit = "ns"; break;
    case 1: unit = "us"; break;
    case 2: unit = "ms"; break;
    default: unit = "s";
  }

  std::stringstream ss;
  ss.precision(3);
  ss << ns << unit;
  return ss.str();
}