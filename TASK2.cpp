#include <iostream>
#include <utility>

// Base case: when there are no arguments, just a newline
void print() {
    std::cout << std::endl;
}

// Variadic template function to handle multiple arguments
template<typename T, typename... Args>
void print(T&& first, Args&&... args) {
    std::cout << std::forward<T>(first);
    if (sizeof...(args) > 0) {
        std::cout << ", ";
    }
    print(std::forward<Args>(args)...);
}

// Example usage
int main() {
    int a = 10;
    double b = 20.5;
    std::string c = "Hello";

    print(a, b, c);             // Print lvalues
    print(42, 3.14, "World");   // Print rvalues
    print(a, 42, std::move(c)); // Mix of lvalue and rvalue
    print("Single argument");

    return 0;
}
