cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 23)

set(LIB_NAME walletpp)

option(BUILD_FOR_IOS "Build for ios" ON)
if (BUILD_FOR_IOS)
    project(${LIB_NAME} LANGUAGES CXX Swift)

    set(CMAKE_OSX_DEPLOYMENT_TARGET 15.0)
    set(CMAKE_CXX_STANDARD 23)

    # Verify that we have a new enough compiler
    if ("${CMAKE_Swift_COMPILER_VERSION}" VERSION_LESS 5.9)
        message(FATAL_ERROR "Bidirectional C++ Interop requires Swift 5.9 or greater. Have ${CMAKE_Swift_COMPILER_VERSION}")
    endif ()

    if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND
            NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
        message(FATAL_ERROR "Project requires building with Clang. Have ${CMAKE_CXX_COMPILER_ID}")
    endif ()

    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

    # Set up swiftrt.o and runtime library search paths
    include(InitializeSwift)
    # cmake/modules/AddSwift.cmake provides the function for creating the Swift to
    # C++ bridging header
    include(AddSwift)

    set(WALLETPP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src")
#    set(WALLETPP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
    set(WALLETPP_LIB_DIR "${CMAKE_SOURCE_DIR}/src")
    set(WALLETPP_SWIFT_SRC_DIR "${CMAKE_SOURCE_DIR}/ios/src")
    set(WALLETPP_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")



    include_directories(${WALLETPP_INCLUDE_DIR})

    _swift_generate_cxx_header_target(
            fibonacci_swift_h
            SwiftFibonacci
            SwiftAdd
            "${CMAKE_CURRENT_BINARY_DIR}/include/fibonacci/fibonacci-swift.h"
            SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/fibonacci.swift"
#            SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/fibonacci.cpp"
            SEARCH_PATHS "${WALLETPP_INCLUDE_DIR}")

    add_library(fibonacci STATIC src/fibonacci.swift src/fibonacci.cpp)
    add_dependencies(fibonacci fibonacci_swift_h)
    target_include_directories(fibonacci PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/src")
#    target_include_directories(fibonacci PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/include")
    set_target_properties(fibonacci PROPERTIES Swift_MODULE_NAME "SwiftFibonacci")
    target_compile_options(fibonacci PUBLIC
            "$<$<COMPILE_LANGUAGE:Swift>:-cxx-interoperability-mode=default>")

    add_subdirectory(${WALLETPP_SWIFT_SRC_DIR})

    message(STATUS "C++ compiler version: ${CMAKE_CXX_COMPILER_VERSION}")
    message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
    message(STATUS "C++ standard version: ${CMAKE_CXX_STANDARD}")

else ()
    # Your existing CMake code for everything else`
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/secp256k1.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/fastpbkdf2.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/csprng.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/sha3iuf.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/libbase58.cmake)

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -Wall -Wextra ")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -Ofast -funroll-loops -flto -march=native -ffast-math")

    option(BUILD_THIRD_PARTY_AS_STATIC_LIBS "Build Static Libraries" OFF)
    option(BUILD_VANITY_PROJECT "Build vanity project" ON)
    option(BUILD_BENCH_PROJECT "Build bench project" OFF)
    option(BUILD_TESTS "Build tests" OFF)

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
            src/secure_vector.cpp
            src/secure_vector.h

            ${THIRD_PARTY_PROJECT_SOURCES}
    )

    target_include_directories(${LIB_NAME}
            PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    target_link_libraries(${LIB_NAME}
            PUBLIC
            secp256k1
            OpenSSL::SSL
            OpenSSL::Crypto
            ${THIRD_PARTY_PROJECT_LIBS}
    )

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
