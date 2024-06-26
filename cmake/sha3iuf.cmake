include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SHA3IUF)

if (BUILD_THIRD_PARTY_AS_STATIC_LIBS)
    add_library(sha3iuf STATIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SHA3IUF/sha3.c)
    set(THIRD_PARTY_PROJECT_LIBS ${THIRD_PARTY_PROJECT_LIBS} sha3iuf)
else ()
    set(THIRD_PARTY_PROJECT_SOURCES ${THIRD_PARTY_PROJECT_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SHA3IUF/sha3.c)
endif ()
