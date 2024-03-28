include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/keccak256)
add_library(keccak256 STATIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/keccak256/keccak256.c)
