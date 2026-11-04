#include <iostream>
#include <cassert>
#include <type_traits>
#include <sstream>
#include "..\Completed_DS\array.hpp" // Assuming your class definition is here

using namespace Container;

// A custom type to verify object tracking, moves, and copies
struct MockElement {
    int id;
    inline static int alive_count = 0;

    MockElement() : id(0) { ++alive_count; }
    MockElement(int val) : id(val) { ++alive_count; }
    MockElement(const MockElement& other) : id(other.id) { ++alive_count; }
    MockElement(MockElement&& other) noexcept : id(other.id) { other.id = -1; ++alive_count; }
    
    MockElement& operator=(const MockElement& other) {
        id = other.id;
        return *this;
    }
    MockElement& operator=(MockElement&& other) noexcept {
        id = other.id;
        other.id = -1;
        return *this;
    }
    
    ~MockElement() { --alive_count; }

    bool operator==(const MockElement& other) const { return id == other.id; }
    bool operator!=(const MockElement& other) const { return id != other.id; }
    
    friend std::ostream& operator<<(std::ostream& os, const MockElement& obj) {
        return os << obj.id;
    }
};

// Satisfy your "template <Custom T>" concept constraint if you have one
// For this test file, let's assume MockElement fits your constraint

void test_constructors_and_properties() {
    // Test default construction
    Array<MockElement> arr1;
    assert(arr1.len() == 0);
    assert(static_cast<bool>(arr1) == false);

    // Test initialization list
    Array<MockElement> arr2 = {10, 20, 30};
    assert(arr2.len() == 3);
    assert(arr2[0].id == 10);
    assert(arr2[2].id == 30);
}

void test_element_modifiers() {
    Array<MockElement> arr;
    
    // Test single append
    bool success = arr.append(MockElement(5));
    assert(success == true);
    assert(arr.len() == 1);
    assert(arr[0].id == 5);

    // Test list append
    arr.append({15, 25});
    assert(arr.len() == 3);
    assert(arr[1].id == 15);
    assert(arr[2].id == 25);

    // Test insertion
    std::cout << arr << std::endl;
    arr.insert(MockElement(99), 1); // Insert 99 at index 1
    assert(arr.len() == 4);
    std::cout << arr << std::endl;
    assert(arr[1].id == 99);
    assert(arr[2].id == 15); // Pushed back
}

void test_operators_and_stream() {
    Array<int> a = {1, 2};
    Array<int> b = {3, 4};
    
    // Test operator+ joining two arrays
    Array<int> c = a + b;
    assert(c.len() == 4);
    assert(c[0] == 1);
    assert(c[3] == 4);

    // Test operator<< streaming output
    std::stringstream ss;
    ss << a;
    assert(ss.str() == "[1, 2]");
}

void test_iterators() {
    Array<int> arr = {100, 200, 300};
    
    std::size_t count = 0;
    int expected[] = {100, 200, 300};
    
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        assert(*it == expected[count]);
        count++;
    }
    assert(count == 3);
}

void test_compile_time_guarantees() {
    // Testing your constant properties with static_assert
    static_assert(Array<int>::npos == static_cast<std::size_t>(-1), "npos must be max value of std::size_t");
    
    // Testing noexcept specifications from interface mapping
    static_assert(noexcept(std::declval<Array<int>>().len()) == true, "len() must be noexcept");
}

int main() {
    std::cout << "Running Array Class Tests...\n";

    test_compile_time_guarantees();
    test_constructors_and_properties();
    test_element_modifiers();
    test_operators_and_stream();
    test_iterators();

    // Verify object memory lifetimes to ensure your pop/remove calls destructors properly
    assert(MockElement::alive_count == 0 && "Memory Leak Alert: Some MockElements were not cleanly destroyed!");

    std::cout << "All tests passed successfully! 🎉\n";
    return 0;
}
