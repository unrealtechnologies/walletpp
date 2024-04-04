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

#ifndef FIBONACCI_FIBONACCI_H
#define FIBONACCI_FIBONACCI_H


#include <string>
#include <iostream>

int fibonacci_cpp(int x);
int fibonacci_cpp2(int x);
const char* generate_entropy(int x);


std::string meow() {
  return "meow";
}

#endif // FIBONACCI_FIBONACCI_H
