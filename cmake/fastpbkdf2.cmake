# OpenSSL
find_package(OpenSSL REQUIRED)

# Include the fastpbkdf2 header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/fastpbkdf2)

# Add the fastpbkdf2 source file
add_library(fastpbkdf2 STATIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/fastpbkdf2/fastpbkdf2.c)

# Ensure fastpbkdf2 and your project link against OpenSSL
target_link_libraries(fastpbkdf2 OpenSSL::Crypto)

