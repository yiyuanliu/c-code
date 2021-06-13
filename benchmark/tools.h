#pragma once

#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <pthread.h>

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
