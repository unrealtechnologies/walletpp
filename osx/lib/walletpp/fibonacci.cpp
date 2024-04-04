//===----------------------------------------------------------------------===//
//
// This source file is part of the Swift open source project
//
// Copyright (c) 2023 Apple Inc. and the Swift project authors.
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://swift.org/LICENSE.txt for license information
//
//===----------------------------------------------------------------------===//

// #include "walletpp/walletpp-swift.h"
#include <iostream>
#include "walletpp/fibonacci.h"
#include "entropy.h"

#include "../../include/walletpp/fibonacci.h"

const char* generate_entropy(int bytes_length) {
    return reinterpret_cast<char*>(walletpp::entropy::generate_entropy(32).data());
    // return "";
    // return {entropy_vec.begin(), entropy_vec.end()};
};

int fibonacci_cpp(int x) {
   std::cout << walletpp::entropy::generate_entropy(32).data() << std::endl;
  std::cout << "x [cpp]: " << x << std::endl;
  if (x <= 1) return 1;
  return fibonacci_cpp(x - 1) + fibonacci_cpp(x - 2);
}

int fibonacci_cpp2(int x) {
    generate_entropy(32);
    std::cout << walletpp::entropy::generate_entropy(32).data() << std::endl;
    std::cout << "x [cpp]: " << x << std::endl;
    if (x <= 1) return 1;
    return fibonacci_cpp(x - 1) + fibonacci_cpp(x - 2);
}


