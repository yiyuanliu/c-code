#include "tools.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/mman.h>

static struct ibv_device *ib_dev = NULL;
static struct ibv_context *ib_ctx;
static struct ibv_pd *ib_pd = NULL;
static bool odp = false;

static constexpr size_t kLoop = 1000;

static void benchmark(char *ptr, size_t len, int loop) {
  uint64_t total = 0;
  for (int i = 0; i < loop; i++) {
    auto begin = clock_ns();
    mlock(ptr, len);
    munlock(ptr, len);
    ptr += len;
    auto end = clock_ns();
    total += end - begin;
  }

  std::cout << format_size(len) << " \t\t"
            << format_latency(total / (double)loop) << std::endl;
}

int main() {
  for (size_t mem = 128; mem <= 16 * 1024ULL * 1024ULL; mem *= 2) {
    char *ptr = (char *)malloc(mem * kLoop);
    if (ptr == nullptr)
      break;
    memset(ptr, 0, mem);
    benchmark(ptr, mem, kLoop);
    free(ptr);
  }

  return 0;
}