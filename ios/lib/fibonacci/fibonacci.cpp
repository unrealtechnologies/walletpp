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

// #include "fibonacci/fibonacci-swift.h"
#include <iostream>

int fibonacci_cpp(const int x) {
  std::cout << "x [cpp]: " << x << std::endl;
  if (x <= 1) return 1;
    return fibonacci_cpp(x - 1) + fibonacci_cpp(x - 2);
  // return SwiftFibonacci::fibonacciSwift(x - 1) + SwiftFibonacci::fibonacciSwift(x - 2);
}

int add_cpp(const int x, const int y) {
  std::cout << "x [cpp]: " << x << std::endl;
  std::cout << "y [cpp]: " << y << std::endl;
    return x + y;
}


// int add_cpp(int x, int y) {
//   return x + y;
//     return SwiftAdd::addSwift(x, y);
// }
