cmake_minimum_required(VERSION 3.26)
project(walletpp_bench)

add_executable(walletpp_bench bench.cpp
        sha256_bench.h
        keccak256_bench.h
        address_generation_bench.h)

get_target_property(WALLETPP_BENCH_INCLUDES walletpp_bench INCLUDE_DIRECTORIES)
message(STATUS "Vanity includes: ${WALLETPP_BENCH_INCLUDES}")

target_link_libraries(walletpp_bench PUBLIC ${LIB_NAME})
