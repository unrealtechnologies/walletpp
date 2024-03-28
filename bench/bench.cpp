//
// Created by Ariel Saldana on 3/16/24.
//

#include "address_generation_bench.h"
#include <chrono>
#include <functional>
#include <iostream>

auto bench_this(const std::function<void()> &bench_func) -> void {
    auto start = std::chrono::high_resolution_clock::now();

    bench_func();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Benchmark took: " << elapsed.count() << " seconds." << std::endl;
}

int main() {
    // bench_this(sha256_bench::bench);
    // bench_this(keccak256::bench);
    bench_this(address_generation_bench::benchmark);
    return 0;
}
