include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/libbase58)
add_library(libbase58 STATIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/libbase58/base58.c)
