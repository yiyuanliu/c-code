#include "tools.h"

#include <infiniband/verbs.h>

#include <iostream>

static struct ibv_device *ib_dev = NULL;
static struct ibv_context *ib_ctx;
static struct ibv_pd *ib_pd = NULL;
static bool odp = false;

static constexpr int kLoop = 100;

static void init() {
    struct ibv_device **dev_list = NULL;
    int num_devices;
    dev_list = ibv_get_device_list(&num_devices);
    if (!dev_list || !num_devices) {
        std::cerr << "Failed to get verbs device list." << std::endl;
        abort();
    }

    std::cout << "Use device " << ibv_get_device_name(dev_list[0]) << std::endl;
    ib_dev = dev_list[0];
    ib_ctx = ibv_open_device(ib_dev);
    if (!ib_ctx) {
        std::cerr << "Failed to open device " << ibv_get_device_name(dev_list[0]) << std::endl;
        abort();
    }

    ibv_free_device_list(dev_list);
    ib_pd = ibv_alloc_pd(ib_ctx);
    if (!ib_pd) {
        std::cerr << "Failed to allocation protection domain." << std::endl;
        abort();
    }

    struct ibv_device_attr_ex attr;
    struct ibv_query_device_ex_input input;
    int ret = ibv_query_device_ex(ib_ctx, &input, &attr);
    if (ret != 0) {
        std::cerr << "Failed to quert device " << ret << "." << std::endl;
        abort();
    }

    std::cout << "Device odp cap: " << std::hex << attr.odp_caps.general_caps << std::dec << std::endl;
    odp = attr.odp_caps.general_caps != 0;
}

static void benchmark(void *ptr, size_t mem_size, int access) {
    uint64_t total_reg = 0;
    uint64_t total_dereg = 0;
    for (int i = 0; i < kLoop; i++) {
        auto begin = clock_ns();
        struct ibv_mr *mr = ibv_reg_mr(ib_pd, ptr, mem_size, access);
        if (!mr) {
            std::cerr << "Failed to reg mr." << std::endl;
            abort();
        }

        auto begin_dereg = clock_ns();
        int ret = ibv_dereg_mr(mr);
        if (ret) {
            std::cerr << "Failed to de reg mr." << std::endl;
            abort();
        }

        auto end = clock_ns();
        total_reg += begin_dereg - begin;
        total_dereg += end - begin_dereg;
    }

    std::cout << mem_size << ", reg " << total_reg / kLoop << "ns, "
              << "dereg " << total_dereg / kLoop << "ns." << std::endl;
}

int main() {
    init();

    void *ptr = malloc(1ULL << 30);
    memset(ptr, 0, 1ULL << 30);

    int access = IBV_ACCESS_REMOTE_READ | IBV_ACCESS_REMOTE_WRITE | IBV_ACCESS_LOCAL_WRITE;
    std::cout << "Benchmark normal:" << std::endl;
    for (size_t mem = 128; mem <= 1ULL << 30; mem *= 4) {
        benchmark(ptr, mem, access);
    }

    if (odp) {
        std::cout << "Benchmark ODP:" << std::endl;
        for (size_t mem = 128; mem <= 1ULL << 30; mem *= 4) {
            benchmark(ptr, mem, access | IBV_ACCESS_ON_DEMAND);
        }
    }

    return 0;
}