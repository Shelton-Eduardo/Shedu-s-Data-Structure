# Shedu's Data Structures (Shedu-s-Data-Structure)

A high-performance, header-only C++ template data structure library designed from scratch to be **fully compatible with the C++ Standard Template Library (STL) design ecosystem**. This library provides optimized, customizable alternatives to standard containers, engineered specifically for generic use cases, maximum throughput, and zero external runtime dependencies.

## 🚀 Key Features

*   **STL-Compatible Architecture:** Implements standard iterator tags, types, concepts, and algorithmic guarantees, allowing seamless drops into any codebase using standard `<algorithm>` routines.
*   **Fully Optimized Templates:** Built from the ground up for modern C++ standards, utilizing aggressive type traits, perfect forwarding, and compile-time evaluation (`constexpr`).
*   **Intuitive & Developer-Friendly Extensions:** Extends standard interfaces with native mathematical operations (`+`, `-`, `*`), stream parsing (`<<`, `>>`), and predictive memory metrics.
*   **Strict Structural Constraints:** Leverages explicit modern C++ concepts to enforce predictable memory signatures and structure constraints at compile-time.

---

## 🛠️ Requirements & Environment

This library is a pure header-only setup that requires a compliant C++20 (or newer) compiler:
*   **Supported Compilers:** GCC 10+, Clang 10+, or MSVC 2019+ (with `/std:c++20` or `/std:c++latest`).
*   **Dependencies:** Standard C++ Library runtime (requires `<type_traits>`, `<concepts>`, `<initializer_list>`, and `<exception>`).

---

## 📦 Quick Start & Usage

Simply drop the header directory into your project include path and consume the containers directly:

```cpp
#include "Container/Array.hpp"
#include <iostream>

int main() {
    // Instantiate using custom concept filtering (sizeof(T) <= 24)
    Container::Array<int> my_array = {4, 2, 8, 1};
    
    // Developer-friendly operators
    my_array += 42; 
    
    // Native stream printing out-of-the-box
    std::cout << "Current Elements: " << my_array << "\n";
    
    return 0;
}
`
