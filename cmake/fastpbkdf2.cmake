# OpenSSL
find_package(OpenSSL REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/fastpbkdf2)


if (BUILD_THIRD_PARTY_AS_STATIC_LIBS)
    add_library(fastpbkdf2 STATIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/fastpbkdf2/fastpbkdf2.c)
    target_link_libraries(fastpbkdf2 OpenSSL::Crypto)
    set(THIRD_PARTY_PROJECT_LIBS ${THIRD_PARTY_PROJECT_LIBS} fastpbkdf2)
else ()
    set(THIRD_PARTY_PROJECT_SOURCES ${THIRD_PARTY_PROJECT_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/fastpbkdf2/fastpbkdf2.c)
endif ()


