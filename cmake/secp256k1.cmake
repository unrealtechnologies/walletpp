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

    # Include the source files directly
    file(GLOB SECP256K1_SOURCES
            ${secp256k1_SOURCE_DIR}/src/precompute_ecmult.c
            ${secp256k1_SOURCE_DIR}/src/precompute_ecmult_gen.c
            ${secp256k1_SOURCE_DIR}/src/precomputed_ecmult.c
            ${secp256k1_SOURCE_DIR}/src/precomputed_ecmult_gen.c
            ${secp256k1_SOURCE_DIR}/src/secp256k1.c
    )

    add_library(secp256k1 STATIC ${SECP256K1_SOURCES})
    target_include_directories(secp256k1 PUBLIC ${secp256k1_SOURCE_DIR}/include)
endif()
