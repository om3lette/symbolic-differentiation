#include "test_groups.hpp"

using namespace Derivative;
using namespace utils;

namespace test {
bool test_lexer(void) {
	bool success = true;
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
			std::vector<Token> answer = {
				{TokenType::EulerConst, "e"}, {TokenType::Operator, "^"},
				{TokenType::LeftParen, "("},  {TokenType::Identifier, "x"},
				{TokenType::Operator, "+"},	  {TokenType::Number, "3"},
				{TokenType::RightParen, ")"}
			};
			return is_lexer_equal(Lexer("e^(x + 3)"), answer);
		},
		"e^(x + 3) (can also be written as exp(x + 3))"
	);
	tester_lexer.register_test(
		[]() {
			// [x, *, sin, (, x, +, 2, )]
			std::vector<Token> answer = {
				{TokenType::EulerConst, "e"}, {TokenType::Operator, "*"},
				{TokenType::LeftParen, "("},  {TokenType::Identifier, "x"},
				{TokenType::Operator, "+"},	  {TokenType::Number, "2"},
				{TokenType::RightParen, ")"}
			};
			return is_lexer_equal(Lexer("e(x + 2)"), answer);
		},
		"e(x + 2)"
	);
	tester_lexer.register_test(
		[]() {
			std::vector<Token> answer = {

				{TokenType::LeftParen, "("},   {TokenType::Identifier, "x"},
				{TokenType::Operator, "+"},	   {TokenType::Number, "2"},
				{TokenType::RightParen, ")"},  {TokenType::Operator, "*"},
				{TokenType::Identifier, "var"}
			};
			return is_lexer_equal(Lexer("(x + 2)var"), answer);
		},
		"(x + 2)var"
	);
	tester_lexer.register_test(
		[]() {
			std::vector<Token> answer = {

				{TokenType::Number, "10"},	 {TokenType::Operator, "*"},
				{TokenType::LeftParen, "("}, {TokenType::Identifier, "x"},
				{TokenType::Operator, "+"},	 {TokenType::Number, "2"},
				{TokenType::RightParen, ")"}
			};
			return is_lexer_equal(Lexer("10(x + 2)"), answer);
		},
		"10(x + 2)"
	);
	tester_lexer.register_test(
		[]() {
			std::vector<Token> answer = {

				{TokenType::LeftParen, "("},  {TokenType::Identifier, "x"},
				{TokenType::Operator, "+"},	  {TokenType::Number, "2"},
				{TokenType::RightParen, ")"}, {TokenType::Operator, "*"},
				{TokenType::Number, "5"}
			};
			return is_lexer_equal(Lexer("(x + 2)5"), answer);
		},
		"(x + 2)5"
	);
	tester_lexer.register_test(
		[]() {
			std::vector<Token> answer = {
				{TokenType::Identifier, "x"}, {TokenType::Operator, "^"},
				{TokenType::Function, "sin"}, {TokenType::LeftParen, "("},
				{TokenType::Identifier, "x"}, {TokenType::Operator, "+"},
				{TokenType::Number, "2"},	  {TokenType::RightParen, ")"},
				{TokenType::Operator, "+"},	  {TokenType::Number, "3"},
				{TokenType::Operator, "*"},	  {TokenType::Identifier, "l"},
			};
			return is_lexer_equal(Lexer("x ^ sin(x + 2) + 3 * l"), answer);
		},
		"x ^ sin(x + 2) + 3 * l"
	);
	tester_lexer.register_test(
		[]() {
			std::vector<Token> answer = {
				{TokenType::Number, "3.14"},
				{TokenType::Operator, "*"},
				{TokenType::Identifier, "x"}
			};
			return is_lexer_equal(Lexer("3.14*x"), answer);
		},
		"3.14 * x (float const)"
	);
	tester_lexer.register_test(
		[]() {
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
	tester_lexer.register_test(
		[]() {
			std::vector<Token> answer = {
				{TokenType::Identifier, "x"}, {TokenType::Operator, "*"},
				{TokenType::LeftParen, "("},  {TokenType::Number, "2"},
				{TokenType::Operator, "+"},	  {TokenType::Number, "2"},
				{TokenType::RightParen, ")"}
			};
			return is_lexer_equal(Lexer("x(2 + 2)"), answer);
		},
		"x(2 + 2)"
	);

	success &= tester_lexer.run_tests();

	//==========================================================================
	test::Tester tester_lexer_format("Lexer formatting");
	tester_lexer_format.register_test(
		[]() {
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
			std::vector<Token> answer = {
				{TokenType::Number, "3"},
				{TokenType::Operator, "*"},
				{TokenType::Complex, "i"}
			};
			return is_lexer_equal(Lexer("3*i"), answer);
		},
		"3*i (no spaces in between | complex)"
	);
	tester_lexer_format.register_test(
		[]() {
			std::vector<Token> answer = {
				{TokenType::Number, "3"},
				{TokenType::Operator, "*"},
				{TokenType::Complex, "i"}
			};
			return is_lexer_equal(Lexer("3i"), answer);
		},
		"3i (no spaces and no operator in between | complex)"
	);
	tester_lexer_format.register_test(
		[]() {
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
			std::vector<Token> answer = {
				{TokenType::Number, "3"},
				{TokenType::Operator, "*"},
				{TokenType::Identifier, "x"}
			};
			return is_lexer_equal(Lexer("3     *   x"), answer);
		},
		"3     *   x (multiple spaces)"
	);
	tester_lexer_format.register_test(
		[]() {
			std::vector<Token> answer = {
				{TokenType::Identifier, "ex"}, {TokenType::Operator, "*"},
				{TokenType::Identifier, "xe"}, {TokenType::Operator, "+"},
				{TokenType::EulerConst, "e"},  {TokenType::Operator, "^"},
				{TokenType::Identifier, "x"}
			};
			return is_lexer_equal(Lexer("eX * Xe + e ^ x"), answer);
		},
		"eX * Xe + e ^ x"
	);
	tester_lexer_format.register_test(
		[]() {
			std::vector<Token> answer = {
				{TokenType::LeftParen, "("},  {TokenType::LeftParen, "("},
				{TokenType::LeftParen, "("},  {TokenType::LeftParen, "("},
				{TokenType::Number, "10"},	  {TokenType::Operator, "+"},
				{TokenType::Number, "5"},	  {TokenType::RightParen, ")"},
				{TokenType::Operator, "*"},	  {TokenType::LeftParen, "("},
				{TokenType::Number, "2"},	  {TokenType::Operator, "-"},
				{TokenType::Number, "4"},	  {TokenType::RightParen, ")"},
				{TokenType::RightParen, ")"}, {TokenType::RightParen, ")"},
				{TokenType::RightParen, ")"}
			};
			return is_lexer_equal(Lexer("((((10 + 5) * (2 - 4))))"), answer);
		},
		"((((10 + 5) * (2 - 4))))"
	);
	tester_lexer_format.register_test(
		[]() {
			std::vector<Token> answer = {{TokenType::Identifier, "e2"}};
			return is_lexer_equal(Lexer("e2"), answer);
		},
		"e2 - variable with digit in name"
	);
	tester_lexer_format.register_test(
		[]() {
			std::vector<Token> answer = {{TokenType::Identifier, "e_2"}};
			return is_lexer_equal(Lexer("e_2"), answer);
		},
		"e_2 - variable with digit in name"
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
	tester_lexer_except.register_exception_test(
		create_lexer_iterator(Lexer("x (23) + 42")),
		"Incorrect syntax. Missing operator after variable"
	);
	tester_lexer_except.register_exception_test(
		create_lexer_iterator(Lexer("(23) y + 42")),
		"Incorrect syntax. Missing operator before variable"
	);
	tester_lexer_except.register_exception_test(
		create_lexer_iterator(Lexer("(23)) y + 42")), "Parenthesis mismatch #1"
	);
	tester_lexer_except.register_exception_test(
		create_lexer_iterator(Lexer("(23) y + (42")), "Parenthesis mismatch #2"
	);
	tester_lexer_except.register_exception_test(
		create_lexer_iterator(Lexer(")23 y + 42")), "Parenthesis mismatch #3"
	);

	success &= tester_lexer_except.run_tests();
	return success;
}
} // namespace test