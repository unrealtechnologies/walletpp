include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/libbase58)

if (BUILD_THIRD_PARTY_AS_STATIC_LIBS)
    add_library(libbase58 STATIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/libbase58/base58.c)
    set(THIRD_PARTY_PROJECT_LIBS ${THIRD_PARTY_PROJECT_LIBS} libbase58)
else ()
    set(THIRD_PARTY_PROJECT_SOURCES ${THIRD_PARTY_PROJECT_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/libbase58/base58.c)
endif ()
