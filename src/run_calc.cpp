#include "../include/calculator.h"
#include <fmt/core.h>

int run_calc(int /*argc*/, char ** /*argv*/) {
    Calculator calc;

    try {
        fmt::print("add(10,4): {}\n", calc.add(10, 4));
        fmt::print("subtract(10,4): {}\n", calc.subtract(10, 4));
        fmt::print("multiply(10,4): {}\n", calc.multiply(10, 4));
        fmt::print("divide(10,4): {}\n", calc.divide(10, 4));
        fmt::print("modulus(10,4): {}\n", calc.modulus(10, 4));
    } catch (const std::exception &e) {
        fmt::println(stderr, "Calculator error: {}", e.what());
        return 1;
    }

    return 0;
}