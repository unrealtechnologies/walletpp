cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)

project(walletpp)
set(LIB_NAME walletpp)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/secp256k1.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/botan.cmake)

add_library(${LIB_NAME} STATIC
        include/walletpp.h
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
        src/secp256k1_context_singleton.h
        src/secp256k1_context_singleton.cpp
        src/bip32.cpp
        src/bip32.h
        src/constants.h
        src/master_key_generator.cpp
        src/master_key_generator.h
        src/key_pair.cpp
        src/key_pair.h
        src/hd_trie.cpp
        src/hd_trie.h
        src/hd_node.cpp
        src/hd_node.h
        src/ethereum_utils.cpp
        src/ethereum_utils.h
        src/extended_key.cpp
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
