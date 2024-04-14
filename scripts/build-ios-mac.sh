#!/bin/bash

# Exit script if any command fails
set -e

# Define directories
BUILD_DIR=$(pwd)/build
CXFRAMEWORK_DIR=$(pwd)/lib.xcframework
LIBDUMP_DIR=$(pwd)/walletpp-lib

# Function to clean up directories
cleanup() {
    echo "Removing existing $1 directory..."
    sudo rm -rf "$1"
}

# Remove the directories if they exist
[ -d "$BUILD_DIR" ] && cleanup "$BUILD_DIR"
[ -d "$CXFRAMEWORK_DIR" ] && cleanup "$CXFRAMEWORK_DIR"
[ -d "$LIBDUMP_DIR" ] && cleanup "$LIBDUMP_DIR"

# Create the build directory
echo "Creating build directory..."
mkdir "$BUILD_DIR"

# Set SSL configuration
echo "Configuring SSL"
export OPENSSL_ROOT_DIR=$(brew --prefix openssl)

# Function to configure, build, install and extract for a specific platform
build_config() {
    local platform=$1
    local config=$2
    cd "$BUILD_DIR"
    echo "Configuring project for $platform..."
    cmake .. -G Xcode -DCMAKE_TOOLCHAIN_FILE=../ios.toolchain.cmake -DPLATFORM="$platform" -DBUILD_IOS=ON

    echo "Building project..."
    cmake --build . --config "$config"

    echo "Installing project..."
    cmake --install . --config "$config"

    echo "Build and installation for $platform completed successfully."
    cd ..
}

# Function to clean build artifacts
clean_build_artifacts() {
    echo "Cleaning build artifacts..."
    cmake --build . --target clean
}

# Build and extract for iOS 64-bit Combined
build_config "OS64COMBINED" "Release"
lipo "$LIBDUMP_DIR/lib/static/libwalletpp.a" -thin arm64 -output "$LIBDUMP_DIR/lib/static/libwalletpp_arm64_ios.a"
lipo "$LIBDUMP_DIR/lib/static/libwalletpp.a" -thin x86_64 -output "$LIBDUMP_DIR/lib/static/libwalletpp_simulator_ios.a"
mv "$LIBDUMP_DIR/lib/static/libwalletpp.a" "$LIBDUMP_DIR/lib/static/libwalletpp_fat_ios.a"

# Clean up and set up for the next build
cleanup "$BUILD_DIR"
mkdir "$BUILD_DIR"

# Build and prepare for macOS Universal
build_config "MAC_UNIVERSAL" "Release"
lipo "$LIBDUMP_DIR/lib/static/libwalletpp.a" -thin arm64 -output "$LIBDUMP_DIR/lib/static/libwalletpp_arm64_mac.a"
lipo "$LIBDUMP_DIR/lib/static/libwalletpp.a" -thin x86_64 -output "$LIBDUMP_DIR/lib/static/libwalletpp_x86_64_mac.a"
mv "$LIBDUMP_DIR/lib/static/libwalletpp.a" "$LIBDUMP_DIR/lib/static/libwalletpp_fat_mac.a"

# Clean build artifacts before creating the XCFramework
cd "$BUILD_DIR"
clean_build_artifacts
cd ..

# Build XCFramework
echo "Building XCFramework..."
xcodebuild -create-xcframework \
           -library "$LIBDUMP_DIR/lib/static/libwalletpp_arm64_ios.a" \
           -headers "$LIBDUMP_DIR/include" \
           -library "$LIBDUMP_DIR/lib/static/libwalletpp_simulator_ios.a" \
           -headers "$LIBDUMP_DIR/include" \
           -library "$LIBDUMP_DIR/lib/static/libwalletpp_arm64_mac.a" \
           -headers "$LIBDUMP_DIR/include" \
           -output "$CXFRAMEWORK_DIR/lib.xcframework"


echo "XCFramework created successfully at $CXFRAMEWORK_DIR"
