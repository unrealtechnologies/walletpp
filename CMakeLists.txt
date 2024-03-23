cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 23)

project(walletpp)
set(LIB_NAME walletpp)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include(${CMAKE_CURRENT_LIST_DIR}/cmake/secp256k1.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/botan.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/fastpbkdf2.cmake)

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
        src/bip44.cpp
        src/bip44.h
        src/coin_type.cpp
        src/coin_type.h
)

target_include_directories(${LIB_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty
        PRIVATE
        ${secp256k1_SOURCE_DIR}/include
        ${OPENSSL_INCLUDE_DIR}
)

message(OPENSSL${OPENSSL_INCLUDE_DIR})

target_link_libraries(${LIB_NAME}
        PUBLIC
        secp256k1
        Botan::botan
        OpenSSL::SSL
        OpenSSL::Crypto
        fastpbkdf2
)

option(BUILD_TESTS "Build tests" OFF)
if (BUILD_TESTS)
    add_subdirectory(test)
endif ()

option(BUILD_VANITY_PROJECT "Build vanity project" ON)
if (BUILD_VANITY_PROJECT)
    add_subdirectory(vanity-address-generator)
    target_link_libraries(vanity_address_generator PUBLIC ${LIB_NAME})
endif ()
