#include "../src/parser/Parser.hpp" // Also includes Lexer.hpp
#include "Tester.hpp"
#include "utils.hpp"

using namespace test;
using namespace Derivative;

int main(void) {
	bool success = true;

	//==========================================================================
	test::Tester tester_lexer("Lexer general");
	tester_lexer.register_test(
		[]() {
			// [x, *, sin, (, x, +, 2, )]
			std::vector<Token> answer = {
				{TokenType::Identifier, "x"}, {TokenType::Operator, "*"},
				{TokenType::Function, "sin"}, {TokenType::LeftParen, "("},
				{TokenType::Identifier, "x"}, {TokenType::Operator, "+"},
				{TokenType::Number, "2"},	  {TokenType::RightParen, ")"}
			};
			return is_lexer_equal(Lexer("x * sin(x + 2)"), answer);
		},
		"x * sin(x + 2)"
	);
	tester_lexer.register_test(
		[]() {
			// [x, *, sin, (, x, +, 2, )]
			std::vector<Token> answer = {
				{TokenType::Function, "exp"}, {TokenType::LeftParen, "("},
				{TokenType::Identifier, "x"}, {TokenType::Operator, "+"},
				{TokenType::Number, "3"},	  {TokenType::RightParen, ")"},
				{TokenType::Operator, "/"},	  {TokenType::LeftParen, "("},
				{TokenType::Function, "sin"}, {TokenType::LeftParen, "("},
				{TokenType::Function, "ln"},  {TokenType::LeftParen, "("},
				{TokenType::Identifier, "x"}, {TokenType::Operator, "+"},
				{TokenType::Identifier, "y"}, {TokenType::RightParen, ")"},
				{TokenType::RightParen, ")"}, {TokenType::RightParen, ")"}
			};
			return is_lexer_equal(
				Lexer("exp(x + 3) / (sin(ln(x + y)))"), answer
			);
		},
		"(x + 3) / (sin(ln(x + y)))"
	);
	tester_lexer.register_test(
		[]() {
			// [x, *, sin, (, x, +, 2, )]
			std::vector<Token> answer = {
				{TokenType::Identifier, "x"}, {TokenType::Operator, "^"},
				{TokenType::Function, "sin"}, {TokenType::LeftParen, "("},
				{TokenType::Identifier, "x"}, {TokenType::Operator, "+"},
				{TokenType::Number, "2"},	  {TokenType::RightParen, ")"},
				{TokenType::Operator, "+"},	  {TokenType::RightParen, "3"},
				{TokenType::Operator, "*"},	  {TokenType::RightParen, "3"},
			};
			return is_lexer_equal(Lexer("x ^ sin(x + 2) + 3 * l"), answer);
		},
		"x ^ sin(x + 2) + 3 * l"
	);
	tester_lexer.register_test(
		[]() {
			// [x, *, sin, (, x, +, 2, )]
			std::vector<Token> answer = {
				{TokenType::Number, "3.14"},
				{TokenType::Operator, "*"},
				{TokenType::Identifier, "x"}
			};
			return is_lexer_equal(Lexer("3*x"), answer);
		},
		"3.14 * x (float const)"
	);
	tester_lexer.register_test(
		[]() {
			// [x, *, sin, (, x, +, 2, )]
			std::vector<Token> answer = {
				{TokenType::Number, "5"},
				{TokenType::Operator, "+"},
				{TokenType::Number, "3.1415"},
				{TokenType::Operator, "*"},
				{TokenType::Complex, "i"}
			};
			return is_lexer_equal(Lexer("5 + 3.1415 * i"), answer);
		},
		"5 + 3.1415 * i (complex numbers)"
	);

	success &= tester_lexer.run_tests();

	//==========================================================================
	test::Tester tester_lexer_format("Lexer formatting");
	tester_lexer_format.register_test(
		[]() {
			// [x, *, sin, (, x, +, 2, )]
			std::vector<Token> answer = {
				{TokenType::Number, "3"},
				{TokenType::Operator, "*"},
				{TokenType::Identifier, "x"}
			};
			return is_lexer_equal(Lexer("3*x"), answer);
		},
		"3*x (no spaces in between)"
	);
	tester_lexer_format.register_test(
		[]() {
			// [x, *, sin, (, x, +, 2, )]
			std::vector<Token> answer = {
				{TokenType::Number, "3"},
				{TokenType::Operator, "*"},
				{TokenType::Identifier, "x"}
			};
			return is_lexer_equal(Lexer("3x"), answer);
		},
		"3x (no spaces and no operator in between)"
	);
	tester_lexer_format.register_test(
		[]() {
			// [x, *, sin, (, x, +, 2, )]
			std::vector<Token> answer = {
				{TokenType::Number, "3"},
				{TokenType::Operator, "*"},
				{TokenType::Complex, "i"}
			};
			return is_lexer_equal(Lexer("3x"), answer);
		},
		"3*i (no spaces in between | complex)"
	);
	tester_lexer_format.register_test(
		[]() {
			// [x, *, sin, (, x, +, 2, )]
			std::vector<Token> answer = {
				{TokenType::Number, "3"},
				{TokenType::Operator, "*"},
				{TokenType::Complex, "i"}
			};
			return is_lexer_equal(Lexer("3x"), answer);
		},
		"3i (no spaces and no operator in between | complex)"
	);
	tester_lexer_format.register_test(
		[]() {
			// [x, *, sin, (, x, +, 2, )]
			std::vector<Token> answer = {
				{TokenType::Number, "3"},
				{TokenType::Operator, "*"},
				{TokenType::Identifier, "x"}
			};
			return is_lexer_equal(Lexer("   3 * x "), answer);
		},
		"'   3 * x '(untrimed string)"
	);
	tester_lexer_format.register_test(
		[]() {
			// [x, *, sin, (, x, +, 2, )]
			std::vector<Token> answer = {
				{TokenType::Number, "3"},
				{TokenType::Operator, "*"},
				{TokenType::Identifier, "x"}
			};
			return is_lexer_equal(Lexer("3     *   x"), answer);
		},
		"3     *   x (multiple spaces)"
	);
	success &= tester_lexer_format.run_tests();

	//==========================================================================
	test::Tester tester_lexer_except("Lexer exceptions");
	tester_lexer_except.register_exception_test(
		create_lexer_iterator(Lexer("sin * x - 10")),
		"Reserved keyword usage (sin)"
	);
	tester_lexer_except.register_exception_test(
		create_lexer_iterator(Lexer("(x + 3) / cos")),
		"Reserved keyword usage (exp)"
	);
	tester_lexer_except.register_exception_test(
		create_lexer_iterator(Lexer("exp - (3)")),
		"Reserved keyword usage (cos)"
	);
	tester_lexer_except.register_exception_test(
		create_lexer_iterator(Lexer("ln (23) + 42")),
		"Reserved keyword usage (ln)"
	);

	success &= tester_lexer_except.run_tests();

	if (!success) throw std::logic_error("\033[1;31mSOME TESTS FAILED!\033[0m");
	std::cout << "\033[1;32m\nALL TESTS PASSED SUCCESSFULLY!\033[0m\n"
			  << std::endl;
	return 0;
}
