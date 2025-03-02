#include "../src/expressions/expressions.hpp"
#include "../src/parser/Parser.hpp" // Also includes Lexer.hpp
#include "Tester.hpp"
#include "groups/test_groups.hpp"
#include "utils.hpp"
#include <limits>

using namespace test;
using namespace Derivative;
using namespace utils;

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
