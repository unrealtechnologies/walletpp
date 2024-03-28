include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SHA3IUF)
add_library(sha3iuf STATIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SHA3IUF/sha3.c)
