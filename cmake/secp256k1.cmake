# Fetch and configure secp256k1
set(SECP256K1_BUILD_TESTS OFF CACHE BOOL "Disable all tests" FORCE)
set(SECP256K1_BUILD_BENCHMARK OFF CACHE BOOL "Disable all benchmarks" FORCE)
set(SECP256K1_BUILD_EXHAUSTIVE_TESTS OFF CACHE BOOL "Disable all exhaustive tests" FORCE)

include(FetchContent)
FetchContent_Declare(
        secp256k1
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/secp256k1
)

FetchContent_GetProperties(secp256k1)
if(NOT secp256k1_POPULATED)
    FetchContent_Populate(secp256k1)
    add_subdirectory(${secp256k1_SOURCE_DIR} ${secp256k1_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

