cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)

project(walletpp)
set(LIB_NAME walletpp)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/secp256k1.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/botan.cmake)

add_library(${LIB_NAME} STATIC
        include/walletpp.h
        src/walletpp.cpp
        src/entropy.cpp
        src/entropy.h
        src/bip39.cpp
        src/bip39.h
        src/crypto_algorithms.cpp
        src/crypto_algorithms.h
        src/wordlist.h
        src/extended_key.h
        src/hd_derivation.cpp
        src/hd_derivation.h
)

target_include_directories(${LIB_NAME}
        PUBLIC
        ${secp256k1_SOURCE_DIR}/include
)

target_link_libraries(${LIB_NAME}
        PRIVATE
        secp256k1
        PUBLIC
        Botan::botan
)

option(BUILD_TESTS "Build tests" ON)

if (BUILD_TESTS)
    add_subdirectory(test)
endif ()
