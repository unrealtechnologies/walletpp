include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/CSPRNG/source)
add_library(csprng STATIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/CSPRNG/source/csprng.cpp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/CSPRNG)
