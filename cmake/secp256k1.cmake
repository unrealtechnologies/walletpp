# Fetch and configure secp256k1
#include(FetchContent)
#FetchContent_Declare(
#        secp256k1
#        GIT_REPOSITORY https://github.com/bitcoin-core/secp256k1.git
#        GIT_TAG master
#)
#
#FetchContent_MakeAvailable(secp256k1)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/secp256k1)
#add_library(secp256k1 STATIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/libbase58/base58.c)

