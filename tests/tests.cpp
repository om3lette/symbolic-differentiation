#include "../src/expressions/expressions.hpp"
#include "../src/parser/Parser.hpp" // Also includes Lexer.hpp
#include "Tester.hpp"
#include "groups/test_groups.hpp"
#include "utils.hpp"
#include <limits>

using namespace test;
using namespace Derivative;
using namespace utils;

// The ultimate prettify test
// make console.build && build/differentiator --diff "(x^4) * sin(x^3) + (e^x) * cos(x^2) + sin(x)/x^2 + cos(x)ln(x + 1)" --by x
// 523 symbols to_string(): (((((((x) ^ (4)) * (((0) * (ln(x)) + ((4) * (1)) / (x)))) * (sin((x) ^ (3))) + ((x) ^ (4)) * ((cos((x) ^ (3))) * (((x) ^ (3)) * (((0) * (ln(x)) + ((3) * (1)) / (x)))))) + (((e^(x)) * (1)) * (cos((x) ^ (2))) + (e^(x)) * (((-1) * (sin((x) ^ (2)))) * (((x) ^ (2)) * (((0) * (ln(x)) + ((2) * (1)) / (x))))))) + ((((cos(x)) * (1)) * ((x) ^ (2)) - (sin(x)) * (((x) ^ (2)) * (((0) * (ln(x)) + ((2) * (1)) / (x)))))) / (((x) ^ (2)) ^ (2))) + ((((-1) * (sin(x))) * (1)) * (ln((x + 1))) + (cos(x)) * (((1) / ((x + 1))) * ((1 + 0)))))
// 381 symbols prettify().to_string() Iteration 1: (((((((x) ^ (4)) * ((4) / (x))) * (sin((x) ^ (3))) + ((x) ^ (4)) * ((cos((x) ^ (3))) * (((x) ^ (3)) * ((3) / (x))))) + ((e^(x)) * (cos((x) ^ (2))) + (e^(x)) * (((-1) * (sin((x) ^ (2)))) * (((x) ^ (2)) * ((2) / (x)))))) + (((cos(x)) * ((x) ^ (2)) - (sin(x)) * (((x) ^ (2)) * ((2) / (x))))) / (((x) ^ (2)) ^ (2))) + (((-1) * (sin(x))) * (ln((x + 1))) + (cos(x)) * ((1) / ((x + 1)))))

int main(void) {
	bool success = true;

	// Leafs
	success &= test_constant();
	success &= test_variable();

	// Operations
	success &= test_addition();
	success &= test_subtraction();
	success &= test_multiplication();
	success &= test_division();
	success &= test_power();

	// Functions
	success &= test_sin();
	success &= test_cos();
	success &= test_ln();
	success &= test_exp();

	// Expression
	success &= test_expression();

	// Parser
	success &= test_lexer();
	success &= test_parser();

	if (!success) throw std::logic_error("\033[1;31mSOME TESTS FAILED!\033[0m");
	std::cout << "\033[1;32m\nALL TESTS PASSED SUCCESSFULLY!\033[0m\n"
			  << std::endl;
	return 0;
}
