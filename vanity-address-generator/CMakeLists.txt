cmake_minimum_required(VERSION 3.27)
project(vanity_address_generator)

add_executable(vanity_address_generator main.cpp
        save_file.h
        encrypt_file.h
        decrypt_file.h
        encryption_vars.h
        wallet_information.h)

get_target_property(VANITY_INCLUDES vanity_address_generator INCLUDE_DIRECTORIES)
message(STATUS "Vanity includes: ${VANITY_INCLUDES}")
