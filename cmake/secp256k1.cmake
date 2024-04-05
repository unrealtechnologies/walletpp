# Fetch and configure secp256k1
#include(FetchContent)
#FetchContent_Declare(
#        secp256k1
#        GIT_REPOSITORY https://github.com/bitcoin-core/secp256k1.git
#        GIT_TAG master
#)
#
#FetchContent_MakeAvailable(secp256k1)

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/secp256k1)
#add_library(secp256k1 STATIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/libbase58/base58.c)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/secp256k1)

if (BUILD_THIRD_PARTY_AS_STATIC_LIBS)
    add_library(secp256k1 STATIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/secp256k1/src/secp256k1.c)
    set(THIRD_PARTY_PROJECT_LIBS ${THIRD_PARTY_PROJECT_LIBS} secp256k1)
    set(THIRD_PARTY_PROJECT_INCLUDES ${THIRD_PARTY_PROJECT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/secp256k1/include)

else ()
    file(GLOB secp256k1_SRC
            "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/secp256k1/include/*.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/secp256k1/src/*.c"
    )

    add_library(secp256k1 STATIC ${secp256k1_SRC})
    set(THIRD_PARTY_PROJECT_INCLUDES ${THIRD_PARTY_PROJECT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/secp256k1/include)
    set(THIRD_PARTY_PROJECT_SOURCES ${THIRD_PARTY_PROJECT_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/secp256k1/src/secp256k1.c)
endif ()
