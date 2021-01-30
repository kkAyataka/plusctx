#include "plusctx/plusctx.hpp"

#include <iostream>
#include <sstream>

// Generates a context stack string from the context stack
std::string get_ctx_string() {
    const auto stack = plusctx::get_context_stack();

    std::ostringstream ss;
    for (const auto c : stack) {
        ss << ">" << c->name;
    }

    return ss.str();
}


void sub1();
void sub2();

int main() {
    PLUSCTX_CTX("main");

    std::cout << get_ctx_string() << std::endl; // >main

    sub1();

    std::cout << get_ctx_string() << std::endl; // >main

    sub2();

    std::cout << get_ctx_string() << std::endl; // >main

    return 0;
}

void sub1() {
    PLUSCTX_CTX("sub1");

    std::cout << get_ctx_string() << std::endl; // >main>sub1

    sub2();
}

void sub2() {
    PLUSCTX_CTX("sub2");

    std::cout << get_ctx_string() << std::endl; // 1st >main>sub1>sub2 // from sub1
                                                // 2nd >main>sub2      // from main
}
