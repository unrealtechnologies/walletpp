
enable_language(CXX)
enable_language(OBJC)

MESSAGE( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS} )
MESSAGE( STATUS "CMAKE_OBJC_FLAGS: " ${CMAKE_OBJC_FLAGS} )

set(CMAKE_SYSTEM_PROCESSOR arm64)

# Add some sanitary checks that the toolchain is actually working!
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

# Hook up XCTest for the supported plaforms (all but WatchOS)
if(NOT PLATFORM MATCHES ".*WATCHOS.*")
    # Use the standard find_package, broken between 3.14.0 and 3.14.4 at least for XCtest...
    find_package(XCTest)
    # Fallback: Try to find XCtest as host package via toochain macro (should always work)
    find_host_package(XCTest REQUIRED)
endif()

# Includes
include_directories(${walletpp_SOURCE_DIR})

# Make sure try_compile() works
include(CheckTypeSize)
check_type_size(time_t SIZEOF_TIME_T)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${walletpp_SOURCE_DIR}/walletpp-lib/walletpp-lib CACHE PATH "Install path" FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
set(CMAKE_INSTALL_PREFIX ${walletpp_SOURCE_DIR}/walletpp-lib/walletpp-lib CACHE PATH "Install path" FORCE)
message("CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}")

# Executable
if(PLATFORM MATCHES "MAC.*")
    set(APP_NAME TestApp)
    add_executable (${APP_NAME} MACOSX_BUNDLE main.cpp)
    set_target_properties(${APP_NAME} PROPERTIES
            BUNDLE True
            MACOSX_BUNDLE_GUI_IDENTIFIER leetal.com.helloworld
            MACOSX_BUNDLE_BUNDLE_NAME helloworld
            MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
    )
    # Link the library with the executable
    target_link_libraries(${APP_NAME} example)
endif()

# Debug symbols set in XCode project
set_xcode_property(walletpp GCC_GENERATE_DEBUGGING_SYMBOLS YES "All")

# Installation
if(PLATFORM MATCHES "MAC.*")
    install(TARGETS ${APP_NAME}
            BUNDLE DESTINATION . COMPONENT Runtime
            RUNTIME DESTINATION bin COMPONENT Runtime
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib/static)

    # Note Mac specific extension .app
    set(APPS "\${CMAKE_INSTALL_PREFIX}/${APP_NAME}.app")

    # Directories to look for dependencies
    set(DIRS ${CMAKE_BINARY_DIR})

    install(CODE "include(BundleUtilities)
    fixup_bundle(\"${APPS}\" \"\" \"${DIRS}\")")

    set(CPACK_GENERATOR "DRAGNDROP")
    include(CPack)
else()
    install(TARGETS walletpp
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib/static)
endif()
install (FILES ${HEADERS} DESTINATION include)

