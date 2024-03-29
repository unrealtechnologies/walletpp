include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/CSPRNG/source)

if (BUILD_THIRD_PARTY_AS_STATIC_LIBS)
    add_library(csprng STATIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/CSPRNG/source/csprng.cpp)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/CSPRNG)
    set(THIRD_PARTY_PROJECT_LIBS ${THIRD_PARTY_PROJECT_LIBS} csprng)
else ()
    set(THIRD_PARTY_PROJECT_SOURCES ${THIRD_PARTY_PROJECT_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/CSPRNG/source/csprng.cpp)
endif ()
