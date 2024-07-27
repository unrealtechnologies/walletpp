cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 23)

project(walletpp LANGUAGES CXX C)
set(LIB_NAME walletpp)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/secp256k1.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/fastpbkdf2.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/csprng.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/sha3iuf.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/libbase58.cmake)

option(BUILD_THIRD_PARTY_AS_STATIC_LIBS "Build Static Libraries" OFF)
option(BUILD_VANITY_PROJECT "Build vanity project" ON)
option(BUILD_BENCH_PROJECT "Build bench project" OFF)
option(BUILD_TESTS "Build tests" OFF)

add_library(${LIB_NAME} STATIC
        include/walletpp.h
        src/entropy.cpp
        include/entropy.h
        src/bip39.cpp
        include/bip39.h
        src/crypto_algorithms.cpp
        include/crypto_algorithms.h
        include/wordlist.h
        include/extended_key.h
        src/hd_derivation.cpp
        include/hd_derivation.h
        include/secp256k1_context_singleton.h
        src/secp256k1_context_singleton.cpp
        src/bip32.cpp
        include/bip32.h
        include/constants.h
        src/master_key_generator.cpp
        include/master_key_generator.h
        src/hd_trie.cpp
        include/hd_trie.h
        src/hd_node.cpp
        include/hd_node.h
        src/ethereum_utils.cpp
        include/ethereum_utils.h
        src/extended_key.cpp
        src/bip44.cpp
        include/bip44.h
        src/coin_type.cpp
        include/coin_type.h
        src/secure_vector.cpp
        include/secure_vector.h

        ${THIRD_PARTY_PROJECT_SOURCES}
)

target_include_directories(${LIB_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${THIRD_PARTY_PROJECT_INCLUDES}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

message("THIRD_PARTY_PROJECT_INCLUDES: ${THIRD_PARTY_PROJECT_INCLUDES}")

target_link_libraries(${LIB_NAME}
        PRIVATE
        secp256k1
        OpenSSL::SSL
        OpenSSL::Crypto
        ${THIRD_PARTY_PROJECT_LIBS}
)

target_compile_features(${LIB_NAME} PUBLIC cxx_std_23)
target_compile_options(${LIB_NAME} PRIVATE -Wall -Wextra $<$<CONFIG:RELEASE>:-O3>)

if (BUILD_TESTS)
    add_subdirectory(test)
endif ()

if (BUILD_VANITY_PROJECT)
    add_subdirectory(vanity-address-generator)
    add_dependencies(vanity_address_generator ${LIB_NAME})
    target_link_libraries(vanity_address_generator PUBLIC ${LIB_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)
endif ()

if (BUILD_BENCH_PROJECT)
    add_subdirectory(bench)
    target_link_libraries(walletpp_bench PUBLIC ${LIB_NAME})
endif ()
