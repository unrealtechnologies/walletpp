include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/cgminer/src)
add_library(cgsha2 STATIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/cgminer/src/sha2.c)
