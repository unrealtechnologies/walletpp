cmake_minimum_required (VERSION 3.18)
project (example-ios)
enable_testing()

enable_language(CXX)
enable_language(OBJC)

message(STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})
message(STATUS "CMAKE_OBJC_FLAGS: " ${CMAKE_OBJC_FLAGS})

include(CheckCXXSymbolExists)
check_cxx_symbol_exists(kqueue sys/event.h HAVE_KQUEUE)
if(NOT HAVE_KQUEUE)
    message(STATUS "kqueue NOT found!")
else()
    message(STATUS "kqueue found!")
endif()

find_library(APPKIT_LIBRARY AppKit)
if (NOT APPKIT_LIBRARY)
    message(STATUS "AppKit.framework NOT found!")
else()
    message(STATUS "AppKit.framework found! ${APPKIT_LIBRARY}")
endif()

find_library(FOUNDATION_LIBRARY Foundation)
if (NOT FOUNDATION_LIBRARY)
    message(STATUS "Foundation.framework NOT found!")
else()
    message(STATUS "Foundation.framework found! ${FOUNDATION_LIBRARY}")
endif()

find_library(UIKIT_LIBRARY UIKit)
if (NOT UIKIT_LIBRARY)
    message(STATUS "UIKit.framework NOT found!")
else()
    message(STATUS "UIKit.framework found! ${UIKIT_LIBRARY}")
endif()

if(NOT PLATFORM MATCHES ".*WATCHOS.*")
    find_package(XCTest)
    find_host_package(XCTest REQUIRED)
endif()

include_directories(${example-ios_SOURCE_DIR})
include(CheckTypeSize)
check_type_size(time_t SIZEOF_TIME_T)

file(GLOB_RECURSE all_cpp_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE all_hpp_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

# Source files
set(SOURCES ${all_cpp_files})
# Headers
set(HEADERS ${all_hpp_files})

# Option for shared or static library
option(BUILD_SHARED "Build as a shared library" OFF)

## Library
#if(BUILD_SHARED)
#    add_library(${LIB_NAME} SHARED ${SOURCES} ${HEADERS})
#    target_link_libraries(example ${FOUNDATION_LIBRARY})
#    target_compile_definitions(example PUBLIC IS_BUILDING_SHARED)
#    message(STATUS "Building shared version...")
#else()
#    add_library(${LIB_NAME}  STATIC ${SOURCES} ${HEADERS})
#    target_link_libraries(example ${FOUNDATION_LIBRARY})
#    message(STATUS "Building static version...")
#endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${example-ios_SOURCE_DIR}/walletpp-lib CACHE PATH "Install path" FORCE)
endif()

set(CMAKE_INSTALL_PREFIX ${example-ios_SOURCE_DIR}/walletpp-lib CACHE PATH "Install path" FORCE)

# Installation of library and headers
install(TARGETS ${LIB_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
install(FILES ${HEADERS} DESTINATION include)

set_xcode_property(${LIB_NAME} GCC_GENERATE_DEBUGGING_SYMBOLS YES "All")
