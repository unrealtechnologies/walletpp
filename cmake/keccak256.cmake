include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/keccak256)

if (BUILD_THIRD_PARTY_AS_STATIC_LIBS)
    add_library(keccak256 STATIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/keccak256/keccak256.c)
    set(THIRD_PARTY_PROJECT_LIBS ${THIRD_PARTY_PROJECT_LIBS} keccak256)
else ()
    set(THIRD_PARTY_PROJECT_SOURCES ${THIRD_PARTY_PROJECT_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/keccak256/keccak256.c)
endif ()
