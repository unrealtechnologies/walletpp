## New Botan Include
include(FetchContent)
FetchContent_Declare(
        botan
        GIT_REPOSITORY https://github.com/randombit/botan.git
        GIT_TAG 3.3.0
)

FetchContent_GetProperties(botan)
string(TOLOWER "botan" lcName)
if (NOT ${lcName}_POPULATED)
    MESSAGE(NOT POPULATED)
    set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_BINARY_DIR}/botan)
    #    find_package(Botan REQUIRED)
    # This downloads botan, but nothing beyond that
    # Ideally we would also use a CONGIFURE step to avoid the extra
    # ExternalProject, but that is explicitly not supported
    FetchContent_Populate(botan)

    include(ExternalProject)
    set(botan_args
#            --minimized-build
#            --enable-modules=hash,bcrypt,scrypt,hex,system_rng,sha2_32,sha2_64,sha3,argon2,rmd160,base58,keccak,tss,system_rng,auto_rng,pubkey
            #--disable-shared
#            --enable-shared
            --cc=gcc
            --prefix=${CMAKE_CURRENT_BINARY_DIR}/botan
    )


    MESSAGE(${${lcName}_SOURCE_DIR})
    MESSAGE(${${lcName}_BINARY_DIR})
    MESSAGE(${CMAKE_CURRENT_BINARY_DIR})
    ExternalProject_Add(botan_project
            SOURCE_DIR ${${lcName}_SOURCE_DIR}
            BINARY_DIR ${${lcName}_BINARY_DIR}
            UPDATE_COMMAND "" # Don't need to update since using tagged release
            CONFIGURE_COMMAND python3 ${${lcName}_SOURCE_DIR}/configure.py ${botan_args}
            BUILD_COMMAND make
            BUILD_BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/botan/lib/libbotan-3.a
            BUILD_ALWAYS true
    )

    add_library(Botan::botan STATIC IMPORTED GLOBAL
            ../include/walletpp.h
            ../src/walletpp.cpp)
    add_dependencies(Botan::botan botan_project)

    # Workaround for INTERFACE_INCLUDE_DIRECTORIES. The problem is the include
    # directory needs to exist at cmake configuration time even though it won't
    # exist until the external project is checked out at build time.
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/botan/include/botan-3)

    set_target_properties(Botan::botan PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/botan/lib/libbotan-3.a
            INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_BINARY_DIR}/botan/include/botan-3
    )

endif ()
