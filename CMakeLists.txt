cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 23)

project(walletpp)
set(LIB_NAME walletpp)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/secp256k1.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/fastpbkdf2.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/csprng.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/sha3iuf.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/libbase58.cmake)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -Wall -Wextra ")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -O3")

option(BUILD_THIRD_PARTY_AS_STATIC_LIBS "Build Static Libraries" OFF)
option(BUILD_VANITY_PROJECT "Build vanity project" OFF)
option(BUILD_BENCH_PROJECT "Build bench project" OFF)
option(BUILD_TESTS "Build tests" ON)

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
        $<INSTALL_INTERFACE:include>  # This is for install targets
        ${THIRD_PARTY_PROJECT_INCLUDES}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
#        ${THIRD_PARTY_PROJECT_INCLUDES}
)

message("THIRD_PARTY_PROJECT_INCLUDES: ${THIRD_PARTY_PROJECT_INCLUDES}")

target_link_libraries(${LIB_NAME}
        PUBLIC
        secp256k1
        OpenSSL::SSL
        OpenSSL::Crypto
        ${THIRD_PARTY_PROJECT_LIBS}
)

#target_include_directories(${LIB_NAME} PUBLIC
#        ${CMAKE_CURRENT_SOURCE_DIR}/include
#)

if (BUILD_IOS)
    set_target_properties(${LIB_NAME} PROPERTIES ENABLE_BITCODE NO)
    set_target_properties(secp256k1 PROPERTIES ENABLE_BITCODE NO)
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/platforms/ios.cmake)

    else ()

    if (BUILD_TESTS)
        add_subdirectory(test)
    endif ()

    if (BUILD_VANITY_PROJECT)
        add_subdirectory(vanity-address-generator)
        add_dependencies(vanity_address_generator ${LIB_NAME})
        target_link_libraries(vanity_address_generator PUBLIC ${LIB_NAME})
    endif ()

    if (BUILD_BENCH_PROJECT)
        add_subdirectory(bench)
        target_link_libraries(walletpp_bench PUBLIC ${LIB_NAME})
    endif ()
endif ()
