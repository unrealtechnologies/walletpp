#!/bin/bash

# Exit script if any command fails
set -e

# Define build directory
BUILD_DIR="build"
CXFRAMEWORK_DIR="lib.xcframework"
LIBDUMP_DIR="walletpp-lib"

# Remove the build directory if it exists
if [ -d "$BUILD_DIR" ]; then
    echo "Removing existing build directory..."
    sudo rm -r "$BUILD_DIR"
fi

if [ -d "$CXFRAMEWORK_DIR" ]; then
    echo "Removing existing build directory..."
    sudo rm -r "$CXFRAMEWORK_DIR"
fi

if [ -d "$LIBDUMP_DIR" ]; then
    echo "Removing existing build directory..."
    sudo rm -r "$LIBDUMP_DIR"
fi

# Create the build directory
echo "Creating build directory..."
mkdir "$BUILD_DIR"

# Change to the build directory
cd "$BUILD_DIR"

echo "Configuring SSL"
export OPENSSL_ROOT_DIR=$(brew --prefix openssl)

# Run CMake commands
echo "Configuring project..."
cmake .. -G Xcode -DCMAKE_TOOLCHAIN_FILE=../ios.toolchain.cmake -DPLATFORM=OS64COMBINED -DBUILD_IOS=ON

echo "Building project..."
cmake --build . --config Release

echo "Installing project..."
cmake --install . --config Release

echo "Build and installation completed successfully."

echo "Attempting to extra arm64 arch from static library"
cd ..

lipo ./walletpp-lib/walletpp-lib/lib/static/libwalletpp.a -thin arm64 -output ./walletpp-lib/walletpp-lib/lib/static/libwalletpp_arm64.a
lipo ./walletpp-lib/walletpp-lib/lib/static/libwalletpp.a -thin  x86_64 -output ./walletpp-lib/walletpp-lib/lib/static/libwalletpp_simulator.a


echo "making xcframework for library"
xcodebuild -create-xcframework \
           -library walletpp-lib/walletpp-lib/lib/static/libwalletpp_arm64.a \
           -headers walletpp-lib/walletpp-lib/include \
           -library walletpp-lib/walletpp-lib/lib/static/libwalletpp_simulator.a \
                      -headers walletpp-lib/walletpp-lib/include \
           -output lib.xcframework


