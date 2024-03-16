cmake_minimum_required(VERSION 3.26)
project(vanity_address_generator)

add_executable(vanity_address_generator main.cpp)

get_target_property(VANITY_INCLUDES vanity_address_generator INCLUDE_DIRECTORIES)
message(STATUS "Vanity includes: ${VANITY_INCLUDES}")

target_link_libraries(vanity_address_generator PUBLIC ${LIB_NAME})
