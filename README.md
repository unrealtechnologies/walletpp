# WalletPP

WalletPP stands for Wallet++ and is a collection of tools for creating cryptocurrency wallets.

## Building image for vanity address generator

```bash
docker build . -t walletpp/vanity-address-generator --progress=plain --no-cache
docker run -i -t docker.io/walletpp/vanity-address-generator
```

```shell 
mkdir build && cd build
```

```shell
cmake -B cmake-build-debug -S ../lib -DCMAKE_TOOLCHAIN_FILE=../ios.toolchain.cmake -DPLATFORM=OS64
cmake --build cmake-build-debug
```

OR

```shell
cd build
export OPENSSL_ROOT_DIR=$(brew --prefix openssl)
cmake .. -G Xcode -DCMAKE_TOOLCHAIN_FILE=../ios.toolchain.cmake -DPLATFORM=OS64COMBINED
cmake .. -G Xcode -DCMAKE_TOOLCHAIN_FILE=../ios.toolchain.cmake -DPLATFORM=OS64COMBINED -DBUILD_IOS=ON
cmake .. -G Xcode -DCMAKE_TOOLCHAIN_FILE=../ios.toolchain.cmake -DPLATFORM=OS64 -DBUILD_IOS=ON

cmake --build . --config Release
cmake --install . --config Release # Necessary to build combined library

xcodebuild -create-xcframework \
           -library walletpp-lib/walletpp-lib/lib/static/libwalletpp.a \
           -headers walletpp-lib/walletpp-lib/include \
           -output lib.xcframework
           

lipo -info /Users/arielsaldana/CLionProjects/walletpp/walletpp-lib/walletpp-lib/lib/static/libwalletpp.a
lipo /Users/arielsaldana/CLionProjects/walletpp/walletpp-lib/walletpp-lib/lib/static/libwalletpp.a -thin arm64 -output /Users/arielsaldana/CLionProjects/walletpp/walletpp-lib/walletpp-lib/lib/static/libwalletpp_arm64.a

xcodebuild -create-xcframework \
           -library walletpp-lib/walletpp-lib/lib/static/libwalletpp_arm64.a \
           -headers walletpp-lib/walletpp-lib/include \
           -output lib.xcframework
```
