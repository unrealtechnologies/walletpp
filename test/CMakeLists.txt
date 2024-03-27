# Fetch and configure Catch2
include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG devel
)
FetchContent_MakeAvailable(Catch2)

# Define tests target
add_executable(walletpp_tests
        test.cpp
        entropy_test.cpp
        bip39_tests.cpp
        bip32_tests.cpp
        master_key_gen_tests.cpp
        hd_derivation_tests.cpp
        hd_trie_tests.cpp
        secure_vector_tests.cpp
)
target_include_directories(walletpp_tests PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(walletpp_tests
        PRIVATE Catch2::Catch2WithMain
        PRIVATE walletpp
)
