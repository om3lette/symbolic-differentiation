#include "../src/expressions/expressions.hpp"
#include "../src/parser/Parser.hpp" // Also includes Lexer.hpp
#include "Tester.hpp"
#include "utils.hpp"
#include <limits>

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

	//==========================================================================
	test::Tester tester_constant("Constant expression");
	tester_constant.register_test(
		is_equal(Constant<long double>(2.0).to_string(), std::string("2")),
		"to_string (long double)"
	);
	tester_constant.register_test(
		is_equal(Constant<long double>(2.25).to_string(), std::string("2.25")),
		"to_string (long double with fraction)"
	);
	tester_constant.register_test(
		is_equal(
			Constant<std::complex<long double>>(std::complex<long double>(1, 1))
				.to_string(),
			std::string("(1 + i)")
		),
		"to_string (complex - real and imag coef = 1)"
	);
	tester_constant.register_test(
		is_equal(
			Constant<std::complex<long double>>(std::complex<long double>(0, 1))
				.to_string(),
			std::string("i")
		),
		"to_string (complex - no real part)"
	);
	tester_constant.register_test(
		is_equal(
			Constant<std::complex<long double>>(std::complex<long double>(1, 0))
				.to_string(),
			std::string("1")
		),
		"to_string (complex - no imag part)"
	);
	tester_constant.register_test(
		is_equal(
			Constant<std::complex<long double>>(std::complex<long double>(0, 0))
				.to_string(),
			std::string("0")
		),
		"to_string (complex = 0)"
	);
	tester_constant.register_test(
		is_equal(
			Constant<std::complex<long double>>(
				std::complex<long double>(-2, -5)
			)
				.to_string(),
			std::string("(-2 - 5i)")
		),
		"to_string (complex - negative imag part)"
	);
	tester_constant.register_test(
		is_equal(Constant<long double>(10.25L).diff("x")->resolve(), 0.0L),
		"diff positive value - 0"
	);
	tester_constant.register_test(
		is_equal(Constant<long double>(-9000.2025L).diff("z")->resolve(), 0.0L),
		"diff negative value - 0"
	);
	tester_constant.register_test(
		is_equal(
			Constant<long double>(100.0L).with_values({{"x", 20}})->resolve(),
			100.0L
		),
		"with_values returns the same expression"
	);
	tester_constant.register_test(
		is_equal(Constant<long double>(100.0L).resolve(), 100.0L),
		"resolve returns the same expression"
	);
	success &= tester_constant.run_tests();

	//==========================================================================
	test::Tester tester_variable("Variable");
	tester_variable.register_test(
		is_equal(Variable<long double>("x").diff("x")->resolve(), 1.0L),
		"d/dx x = 1"
	);
	tester_variable.register_test(
		is_equal(Variable<long double>("x").diff("y")->resolve(), 0.0L),
		"d/dy x = 0"
	);
	tester_variable.register_test(
		is_equal(
			Variable<long double>("x").with_values({{"x", 42.0L}})->resolve(),
			42.0L
		),
		"x with_values(x = 42)"
	);
	tester_variable.register_test(
		is_equal(
			Variable<std::complex<long double>>("z")
				.with_values({{"z", std::complex<long double>(2, 2)}})
				->resolve(),
			std::complex<long double>(2, 2)
		),
		"z with_values(z = 2 + 2i)"
	);
	tester_variable.register_test(
		is_equal(
			Variable<std::complex<long double>>("z")
				.with_values({{"z", std::complex<long double>(2, 2)}})
				->to_string(),
			std::string("(2 + 2i)")
		),
		"z with_values(z = 2 + 2i)->to_string()"
	);
	tester_variable.register_test(
		is_equal(
			Variable<long double>("long_var").to_string(),
			std::string("long_var")
		),
		"to_string long_var"
	);
	tester_variable.register_exception_test(
		[]() {
			Variable<long double>("x").with_values({{"y", 2.0L}})->resolve();
		},
		"x with_values(y) into resolve (runtime error)"
	);

	success &= tester_variable.run_tests();

	//==========================================================================
	test::Tester tester_addition("Addition operation");
	// clang-format off
	tester_addition.register_test(
		is_equal(
			std::make_shared<AddOp<long double>>(
				std::make_shared<Constant<long double>>(25.0L),
				std::make_shared<Variable<long double>>("x")
			)->to_string(),
			std::string("(25 + x)")
		),
		"to_string 25 + x"
	);
	tester_addition.register_test(
		is_equal(
			std::make_shared<AddOp<std::complex<long double>>>(
				std::make_shared<Constant<std::complex<long double>>>(std::complex<long double>(25.0L, 10.25L)),
				std::make_shared<Variable<std::complex<long double>>>("x")
			)->to_string(),
			std::string("((25 + 10.25i) + x)")
		),
		"to_string complex (25 + 10.25i) + x"
	);
	tester_addition.register_test(
		is_equal(
			std::make_shared<AddOp<long double>>(
				std::make_shared<Constant<long double>>(25.0L),
				std::make_shared<Variable<long double>>("x")
			)->with_values({{"x", 42.0L}})->resolve(),
			67.0L
		),
		"with_values()->resolve() 25 + 42 = 67"
	);
	tester_addition.register_test(
		is_equal(
			std::make_shared<AddOp<long double>>(
				std::make_shared<Constant<long double>>(25.0L),
				std::make_shared<Variable<long double>>("x")
			)->with_values({{"x", -42.0L}})->resolve(),
			-17.0L
		),
		"with_values()->resolve() 25 + -42 = -17"
	);
	tester_addition.register_test(
		is_equal(
			std::make_shared<AddOp<long double>>(
				std::make_shared<AddOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Variable<long double>>("y")
				),
				std::make_shared<Constant<long double>>(12.0L)
			)->with_values({{"x", 22.0L}, {"y", 9.0L}})->resolve(),
			43.0L
		),
		"with_values(x = 22, y = 9)->resolve() x + y + 12 = 43"
	);
	tester_addition.register_test(
		is_equal(
			std::make_shared<AddOp<std::complex<long double>>>(
				std::make_shared<Constant<std::complex<long double>>>(25.0L),
				std::make_shared<Variable<std::complex<long double>>>("x")
			)->with_values({{"x", std::complex<long double>(2.0L, 2.0L)}})->resolve(),
			std::complex<long double>(27.0L, 2.0L)
		),
		"with_values()->resolve() complex 25 + (2 + 2i) = 27 + 2i"
	);
	tester_addition.register_test(
		is_equal(
			std::make_shared<AddOp<std::complex<long double>>>(
				std::make_shared<Constant<std::complex<long double>>>(25.0L),
				std::make_shared<Variable<std::complex<long double>>>("x")
			)->with_values({{"x", std::complex<long double>(-2.0L, -2.0L)}})->resolve(),
			std::complex<long double>(23.0L, -2.0L)
		),
		"with_values()->resolve() complex 25 + (-2 - 2i) = 27 + 2i"
	);
	tester_addition.register_test(
		is_equal(
			std::make_shared<AddOp<long double>>(
				std::make_shared<AddOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Variable<long double>>("y")
				),
				std::make_shared<Constant<long double>>(12.0L)
			)->diff("x")->resolve(),
			std::make_shared<Variable<long double>>("x")->diff("x")->resolve()
		),
		"diff()->resolve() d/dx x + y + 12 = d/dx x"
	);
	tester_addition.register_test(
		is_equal(
			std::make_shared<AddOp<long double>>(
				std::make_shared<AddOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Variable<long double>>("y")
				),
				std::make_shared<Constant<long double>>(12.0L)
			)->diff("x")->resolve(),
			1.0L
		),
		"diff()->resolve() d/dx x + y + 12 = 1"
	);
	// clang-format on
	success &= tester_addition.run_tests();

	//==========================================================================
	test::Tester tester_subtraction("Subtraction operation");
	// clang-format off
	tester_subtraction.register_test(
		is_equal(
			std::make_shared<SubOp<long double>>(
				std::make_shared<Constant<long double>>(25.0L),
				std::make_shared<Variable<long double>>("x")
			)->to_string(),
			std::string("(25 - x)")
		),
		"to_string 25 - x"
	);
	tester_subtraction.register_test(
		is_equal(
			std::make_shared<SubOp<std::complex<long double>>>(
				std::make_shared<Constant<std::complex<long double>>>(std::complex<long double>(25.0L, 10.25L)),
				std::make_shared<Variable<std::complex<long double>>>("x")
			)->to_string(),
			std::string("((25 + 10.25i) - x)")
		),
		"to_string complex (25 + 10.25i) - x"
	);
	tester_subtraction.register_test(
		is_equal(
			std::make_shared<SubOp<long double>>(
				std::make_shared<Constant<long double>>(25.0L),
				std::make_shared<Variable<long double>>("x")
			)->with_values({{"x", 42.0L}})->resolve(),
			-17.0L
		),
		"with_values()->resolve() 25 - 42 = -17"
	);
	tester_subtraction.register_test(
		is_equal(
			std::make_shared<SubOp<long double>>(
				std::make_shared<Constant<long double>>(25.0L),
				std::make_shared<Variable<long double>>("x")
			)->with_values({{"x", -42.0L}})->resolve(),
			67.0L
		),
		"with_values()->resolve() 25 - -42 = 67"
	);
	tester_subtraction.register_test(
		is_equal(
			std::make_shared<SubOp<long double>>(
				std::make_shared<SubOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Variable<long double>>("y")
				),
				std::make_shared<Constant<long double>>(12.0L)
			)->with_values({{"x", 22.0L}, {"y", 9.0L}})->resolve(),
			1.0L
		),
		"with_values(x = 22, y = 9)->resolve() x - y - 12 = 1"
	);
	tester_subtraction.register_test(
		is_equal(
			std::make_shared<SubOp<std::complex<long double>>>(
				std::make_shared<Constant<std::complex<long double>>>(25.0L),
				std::make_shared<Variable<std::complex<long double>>>("x")
			)->with_values({{"x", std::complex<long double>(2.0L, 2.0L)}})->resolve(),
			std::complex<long double>(23.0L, -2.0L)
		),
		"with_values()->resolve() complex 25 - (2 + 2i) = 23 - 2i"
	);
	tester_subtraction.register_test(
		is_equal(
			std::make_shared<SubOp<std::complex<long double>>>(
				std::make_shared<Constant<std::complex<long double>>>(25.0L),
				std::make_shared<Variable<std::complex<long double>>>("x")
			)->with_values({{"x", std::complex<long double>(-2.0L, -2.0L)}})->resolve(),
			std::complex<long double>(27.0L, 2.0L)
		),
		"with_values()->resolve() complex 25 - (-2 - 2i) = 27 + 2i"
	);
	tester_subtraction.register_test(
		is_equal(
			std::make_shared<SubOp<long double>>(
				std::make_shared<SubOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Variable<long double>>("y")
				),
				std::make_shared<Constant<long double>>(12.0L)
			)->diff("x")->resolve(),
			std::make_shared<Variable<long double>>("x")->diff("x")->resolve()
		),
		"diff()->resolve() d/dx x - y - 12 = d/dx x"
	);
	tester_subtraction.register_test(
		is_equal(
			std::make_shared<SubOp<long double>>(
				std::make_shared<SubOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Variable<long double>>("y")
				),
				std::make_shared<Constant<long double>>(12.0L)
			)->diff("x")->resolve(),
			1.0L
		),
		"diff()->resolve() d/dx x - y - 12 = 1"
	);
	// clang-format on
	success &= tester_subtraction.run_tests();

	//==========================================================================
	test::Tester tester_multiplication("Multiplication operation");
	// clang-format off
	tester_multiplication.register_test(
		is_equal(
			std::make_shared<MultOp<long double>>(
				std::make_shared<Constant<long double>>(25.0L),
				std::make_shared<Variable<long double>>("x")
			)->to_string(),
			std::string("(25) * (x)")
		),
		"to_string 25 * x"
	);
	tester_multiplication.register_test(
		is_equal(
			std::make_shared<MultOp<std::complex<long double>>>(
				std::make_shared<Constant<std::complex<long double>>>(std::complex<long double>(25.0L, 10.25L)),
				std::make_shared<Variable<std::complex<long double>>>("x")
			)->to_string(),
			std::string("((25 + 10.25i)) * (x)")
		),
		"to_string complex (25 + 10.25i) * x"
	);
	tester_multiplication.register_test(
		is_equal(
			std::make_shared<MultOp<long double>>(
				std::make_shared<Constant<long double>>(25.0L),
				std::make_shared<Variable<long double>>("x")
			)->with_values({{"x", 42.0L}})->resolve(),
			1050.0L
		),
		"with_values()->resolve() 25 * 42 = 1050"
	);
	tester_multiplication.register_test(
		is_equal(
			std::make_shared<MultOp<long double>>(
				std::make_shared<Constant<long double>>(25.0L),
				std::make_shared<Variable<long double>>("x")
			)->with_values({{"x", -42.0L}})->resolve(),
			-1050.0L
		),
		"with_values()->resolve() 25 * -42 = -1050"
	);
	tester_multiplication.register_test(
		is_equal(
			std::make_shared<MultOp<long double>>(
				std::make_shared<MultOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Variable<long double>>("y")
				),
				std::make_shared<Constant<long double>>(12.0L)
			)->with_values({{"x", 22.0L}, {"y", 9.0L}})->resolve(),
			2376.0L
		),
		"with_values(x = 22, y = 9)->resolve() x * y * 12 = 2376"
	);
	tester_multiplication.register_test(
		is_equal(
			std::make_shared<MultOp<std::complex<long double>>>(
				std::make_shared<Constant<std::complex<long double>>>(25.0L),
				std::make_shared<Variable<std::complex<long double>>>("x")
			)->with_values({{"x", std::complex<long double>(2.0L, 2.0L)}})->resolve(),
			std::complex<long double>(50.0L, 50.0L)
		),
		"with_values()->resolve() complex 25 * (2 + 2i) = 50 + 50i"
	);
	tester_multiplication.register_test(
		is_equal(
			std::make_shared<MultOp<std::complex<long double>>>(
				std::make_shared<Constant<std::complex<long double>>>(25.0L),
				std::make_shared<Variable<std::complex<long double>>>("x")
			)->with_values({{"x", std::complex<long double>(-2.0L, -2.0L)}})->resolve(),
			std::complex<long double>(-50.0L, -50.0L)
		),
		"with_values()->resolve() complex 25 * (-2 - 2i) = -50 - 50i"
	);
	tester_multiplication.register_test(
		is_equal(
			std::make_shared<MultOp<long double>>(
				std::make_shared<Variable<long double>>("x"),
				std::make_shared<Constant<long double>>(12.0L)
			)->diff("x")->to_string(),
			std::string("((1) * (12) + (x) * (0))")
		),
		"diff()->resolve() d/dx 12x = 12"
	);
	tester_multiplication.register_test(
		is_equal(
			std::make_shared<MultOp<long double>>(
				std::make_shared<MultOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Variable<long double>>("y")
				),
				std::make_shared<Constant<long double>>(12.0L)
			)->diff("y")->to_string(),
			std::string("((((0) * (y) + (x) * (1))) * (12) + ((x) * (y)) * (0))")
		),
		"diff()->resolve() d/dy x * y * 12 = 12x"
	);
	// clang-format on
	success &= tester_multiplication.run_tests();

	//==========================================================================
	test::Tester tester_ln("Ln function");
	// clang-format off
	tester_ln.register_test(
		is_equal(
			std::make_shared<LnFunc<long double>>(
				std::make_shared<Variable<long double>>("x")
			)->to_string(),
			std::string("ln(x)")
		),
		"to_string ln(x)"
	);
	tester_ln.register_test(
		is_equal(
			std::make_shared<LnFunc<long double>>(
				std::make_shared<AddOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Constant<long double>>(1.0L)
				)
			)->to_string(),
			std::string("ln((x + 1))")
		),
		"to_string ln(x + 1)"
	);
	tester_ln.register_test(
		is_equal(
			std::make_shared<LnFunc<long double>>(
					std::make_shared<Variable<long double>>("x")
			)->with_values({{"x", e}})->resolve(),
			1.0L
		),
		"ln(e) = 1"
	);
	tester_ln.register_test(
		is_within_tolerance(
			std::make_shared<LnFunc<long double>>(
					std::make_shared<Variable<long double>>("x")
			)->with_values({{"x", std::pow(e, 15)}})->resolve(),
			15.0L
		),
		"ln(e) = 1"
	);
	tester_ln.register_test(
		is_equal(
			std::make_shared<LnFunc<long double>>(
				std::make_shared<MultOp<long double>>(
					std::make_shared<Constant<long double>>(10000.0L),
					std::make_shared<Variable<long double>>("x")
				)
			)->diff("x")->with_values({{"x", 1}})->resolve(),
			1.0L
		),
		"ln'(10000 * x) = 1/x"
	);
	tester_ln.register_test(
		is_equal(
			std::make_shared<LnFunc<long double>>(
				std::make_shared<AddOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Constant<long double>>(10.0L)
				)
			)->diff("x")->with_values({{"x", 10}})->resolve(),
			0.05L
		),
		"ln'(x + 10) = 1/(x + 10)"
	);
	// clang-format on
	success &= tester_ln.run_tests();

	//==========================================================================
	test::Tester tester_power("Power operation");
	// clang-format off
	tester_power.register_test(
		is_equal(
			std::make_shared<PowOp<long double>>(
				std::make_shared<Constant<long double>>(25.0L),
				std::make_shared<Variable<long double>>("x")
			)->to_string(),
			std::string("(25) ^ (x)")
		),
		"to_string 25 ^ x"
	);
	tester_power.register_test(
		is_equal(
			std::make_shared<PowOp<std::complex<long double>>>(
				std::make_shared<Constant<std::complex<long double>>>(std::complex<long double>(25.0L, 10.25L)),
				std::make_shared<Variable<std::complex<long double>>>("x")
			)->to_string(),
			std::string("((25 + 10.25i)) ^ (x)")
		),
		"to_string complex (25 + 10.25i) ^ x"
	);
	tester_power.register_test(
		is_equal(
			std::make_shared<PowOp<long double>>(
				std::make_shared<Constant<long double>>(25.0L),
				std::make_shared<Variable<long double>>("x")
			)->with_values({{"x", 2.0L}})->resolve(),
			625.0L
		),
		"with_values()->resolve() 25 ^ 2 = 625"
	);
	tester_power.register_test(
		is_within_tolerance(
			std::make_shared<PowOp<long double>>(
				std::make_shared<Constant<long double>>(25.0L),
				std::make_shared<Variable<long double>>("x")
			)->with_values({{"x", -2.0L}})->resolve(),
			0.0016L
		),
		"with_values()->resolve() 25 ^ -2 = 0.0016"
	);
	tester_power.register_test(
		is_equal(
			std::make_shared<PowOp<long double>>(
				std::make_shared<PowOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Variable<long double>>("y")
				),
				std::make_shared<Constant<long double>>(2.0L)
			)->with_values({{"x", 2.0L}, {"y", 3.0L}})->resolve(),
			64.0L
		),
		"with_values(x = 2, y = 3)->resolve() x ^ y ^ 2 = 64"
	);
	tester_power.register_test(
		is_equal_complex(
			std::make_shared<PowOp<std::complex<long double>>>(
				std::make_shared<Variable<std::complex<long double>>>("x"),
				std::make_shared<Constant<std::complex<long double>>>(3.0L)
			)->with_values({{"x", std::complex<long double>(2.0L, 2.0L)}})->resolve(),
			std::complex<long double>(-16.0L, 16.0L)
		),
		"with_values()->resolve() complex (2 + 2i)^3 = -16 + 16i"
	);
	tester_power.register_test(
		is_equal_complex(
			std::make_shared<PowOp<std::complex<long double>>>(
				std::make_shared<Variable<std::complex<long double>>>("x"),
				std::make_shared<Constant<std::complex<long double>>>(3.0L)
			)->with_values({{"x", std::complex<long double>(-2.0L, -2.0L)}})->resolve(),
			std::complex<long double>(16.0L, -16.0L)
		),
		"with_values()->resolve() complex (-2 - 2i)^3 = 16 - 16i"
	);
	// Assert 3 * x^2 with x = 4 is 48
	tester_power.register_test(
		is_equal(
			std::make_shared<PowOp<long double>>(
				std::make_shared<Variable<long double>>("x"),
				std::make_shared<Constant<long double>>(3.0L)
			)->diff("x")->with_values({{"x", 4.0L}})->resolve(),
			48.0L
		),
		"diff()->resolve() d/dx x^3 = 3x^2"
	);
	// Assert 3(x + 1)^2 with x = 4 is 75
	tester_power.register_test(
		is_equal(
			std::make_shared<PowOp<long double>>(
				std::make_shared<AddOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Constant<long double>>(1.0L)
				),
				std::make_shared<Constant<long double>>(3.0L)
			)->diff("x")->with_values({{"x", 4}})->resolve(),
			75.0L
		),
		"diff()->resolve() d/dy (x + 1)^3 = 3(x + 1)^2"
	);
	// clang-format on
	success &= tester_power.run_tests();

	//==========================================================================
	test::Tester tester_division("Division operation");
	// clang-format off
	tester_division.register_test(
		is_equal(
			std::make_shared<DivOp<long double>>(
				std::make_shared<Constant<long double>>(25.0L),
				std::make_shared<Variable<long double>>("x")
			)->to_string(),
			std::string("(25) / (x)")
		),
		"to_string 25 / x"
	);
	tester_division.register_test(
		is_equal(
			std::make_shared<DivOp<std::complex<long double>>>(
				std::make_shared<Constant<std::complex<long double>>>(std::complex<long double>(25.0L, 10.25L)),
				std::make_shared<Variable<std::complex<long double>>>("x")
			)->to_string(),
			std::string("((25 + 10.25i)) / (x)")
		),
		"to_string complex (25 + 10.25i) / x"
	);
	tester_division.register_test(
		is_equal(
			std::make_shared<DivOp<long double>>(
				std::make_shared<Constant<long double>>(25.0L),
				std::make_shared<Variable<long double>>("x")
			)->with_values({{"x", 5.0L}})->resolve(),
			5.0L
		),
		"with_values()->resolve() 25 / 5 = 5"
	);
	tester_division.register_test(
		is_equal(
			std::make_shared<DivOp<long double>>(
				std::make_shared<Constant<long double>>(25.0L),
				std::make_shared<Variable<long double>>("x")
			)->with_values({{"x", -5.0L}})->resolve(),
			-5.0L
		),
		"with_values()->resolve() 25 / -5 = -5"
	);
	tester_division.register_test(
		is_equal(
			std::make_shared<DivOp<long double>>(
				std::make_shared<DivOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Variable<long double>>("y")
				),
				std::make_shared<Constant<long double>>(12.0L)
			)->with_values({{"x", 24.0L}, {"y", 2.0L}})->resolve(),
			1.0L
		),
		"with_values(x = 24, y = 2)->resolve() x / y / 12 = 1"
	);
	tester_division.register_test(
		is_equal(
			std::make_shared<DivOp<std::complex<long double>>>(
				std::make_shared<Constant<std::complex<long double>>>(25.0L),
				std::make_shared<Variable<std::complex<long double>>>("x")
			)->with_values({{"x", std::complex<long double>(2.0L, 2.0L)}})->resolve(),
			std::complex<long double>(6.25L, -6.25L)
		),
		"with_values()->resolve() complex 25 / (2 + 2i) = 6.25 - 6.25i"
	);
	tester_division.register_test(
		is_equal(
			std::make_shared<DivOp<std::complex<long double>>>(
				std::make_shared<Constant<std::complex<long double>>>(25.0L),
				std::make_shared<Variable<std::complex<long double>>>("x")
			)->with_values({{"x", std::complex<long double>(-2.0L, -2.0L)}})->resolve(),
			std::complex<long double>(-6.25L, 6.25L)
		),
		"with_values()->resolve() complex 25 / (2 + 2i) = -6.25 + -6.25i"
	);
	tester_division.register_test(
		is_equal(
			std::make_shared<DivOp<long double>>(
				std::make_shared<Variable<long double>>("x"),
				std::make_shared<Constant<long double>>(10.0L)
			)->diff("x")->with_values({{"x", 999}})->resolve(),
			0.1L
		),
		"diff()->resolve() d/dx x/10 = 0.1"
	);
	// Answer is 12/y. Assert that if y = 2 than answer is 6
	tester_division.register_test(
		is_equal(
			std::make_shared<DivOp<long double>>(
				std::make_shared<Variable<long double>>("x"),
				std::make_shared<DivOp<long double>>(
					std::make_shared<Variable<long double>>("y"),
					std::make_shared<Constant<long double>>(12.0L)
				)
			)->diff("x")->with_values({{"x", 999},  {"y", 2}})->resolve(),
			6.0L
		),
		"diff()->resolve() d/dy x / y / 12 = 12/y"
	);
	// clang-format on
	success &= tester_division.run_tests();

	//==========================================================================
	test::Tester tester_sin("Sin function");
	// clang-format off
	tester_sin.register_test(
		is_equal(
			std::make_shared<SinFunc<long double>>(
				std::make_shared<Variable<long double>>("x")
			)->to_string(),
			std::string("sin(x)")
		),
		"to_string sin(x)"
	);
	tester_sin.register_test(
		is_equal(
			std::make_shared<SinFunc<long double>>(
				std::make_shared<MultOp<long double>>(
					std::make_shared<Constant<long double>>(4.0L),
					std::make_shared<Variable<long double>>("x")
				)
			)->to_string(),
			std::string("sin((4) * (x))")
		),
		"to_string sin(4 * x)"
	);
	tester_sin.register_test(
		is_within_tolerance(
			std::make_shared<SinFunc<long double>>(
				std::make_shared<MultOp<long double>>(
					std::make_shared<Constant<long double>>(4.0L),
					std::make_shared<Variable<long double>>("x")
				)
			)->with_values({{"x", pi}})->resolve(),
			0.0L
		),
		"with_values(x = pi) sin(4x) = 0"
	);
	tester_sin.register_test(
		is_within_tolerance(
			std::make_shared<SinFunc<long double>>(
				std::make_shared<DivOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Constant<long double>>(6)
				)
			)->with_values({{"x", pi}})->resolve(),
			0.5L
		),
		"with_values(x = pi) sin(x/6) = 0.5"
	);
	tester_sin.register_test(
		is_within_tolerance(
			std::make_shared<SinFunc<long double>>(
				std::make_shared<DivOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Constant<long double>>(3)
				)
			)->diff("x")->with_values({{"x", pi}})->resolve(),
			1.0L / 6
		),
		"d/dx sin(x/3) = cos(x/3)/3"
	);
	// clang-format on
	success &= tester_sin.run_tests();

	//==========================================================================
	test::Tester tester_cos("Cos function");
	// clang-format off
		tester_cos.register_test(
			is_equal(
				std::make_shared<CosFunc<long double>>(
					std::make_shared<Variable<long double>>("x")
				)->to_string(),
				std::string("cos(x)")
			),
			"to_string cos(x)"
		);
		tester_cos.register_test(
			is_equal(
				std::make_shared<CosFunc<long double>>(
					std::make_shared<MultOp<long double>>(
						std::make_shared<Constant<long double>>(4.0L),
						std::make_shared<Variable<long double>>("x")
					)
				)->to_string(),
				std::string("cos((4) * (x))")
			),
			"to_string cos(4 * x)"
		);
		tester_cos.register_test(
			is_within_tolerance(
				std::make_shared<CosFunc<long double>>(
					std::make_shared<MultOp<long double>>(
						std::make_shared<Constant<long double>>(4.0L),
						std::make_shared<Variable<long double>>("x")
					)
				)->with_values({{"x", pi/8}})->resolve(),
				0.0L
			),
			"with_values(x = pi/8) cos(4x) = 0"
		);
		tester_cos.register_test(
			is_within_tolerance(
				std::make_shared<CosFunc<long double>>(
					std::make_shared<DivOp<long double>>(
						std::make_shared<Variable<long double>>("x"),
						std::make_shared<Constant<long double>>(3)
					)
				)->with_values({{"x", pi}})->resolve(),
				0.5L
			),
			"with_values(x = pi) cos(x/3) = 0.5"
		);
		tester_cos.register_test(
			is_within_tolerance(
				std::make_shared<CosFunc<long double>>(
					std::make_shared<DivOp<long double>>(
						std::make_shared<Variable<long double>>("x"),
						std::make_shared<Constant<long double>>(6)
					)
				)->diff("x")->with_values({{"x", pi}})->resolve(),
				-1.0L / 12
			),
			"d/dx cos(x/6) = -1 * sin(x/6)/6"
		);
	// clang-format on
	success &= tester_cos.run_tests();

	//==========================================================================
	test::Tester tester_exp("Exp function");
	// clang-format off
	tester_exp.register_test(
		is_equal(
			std::make_shared<ExpFunc<long double>>(
				std::make_shared<Variable<long double>>("x")
			)->to_string(),
			std::string("e^(x)")
		),
		"to_string e^x"
	);
	tester_exp.register_test(
		is_equal(
			std::make_shared<ExpFunc<long double>>(
				std::make_shared<MultOp<long double>>(
					std::make_shared<Constant<long double>>(4.0L),
					std::make_shared<Variable<long double>>("x")
				)
			)->to_string(),
			std::string("e^((4) * (x))")
		),
		"to_string e^(4 * x)"
	);
	tester_exp.register_test(
		is_within_tolerance(
			std::make_shared<ExpFunc<long double>>(
				std::make_shared<MultOp<long double>>(
					std::make_shared<Constant<long double>>(4.0L),
					std::make_shared<Variable<long double>>("x")
				)
			)->with_values({{"x", 2}})->resolve(),
			std::pow(e, 8)
		),
		"with_values(x = 2) e^(4x) = e^8"
	);
	tester_exp.register_test(
		is_within_tolerance(
			std::make_shared<ExpFunc<long double>>(
				std::make_shared<DivOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Constant<long double>>(3)
				)
			)->with_values({{"x", -1}})->resolve(),
			std::pow(e, -1.0L / 3)
		),
		"with_values(x = -1) e^(x/3) = 1/e^3"
	);
	long double e_by_six = e / 6.0L;
	tester_exp.register_test(
		is_within_tolerance(
			std::make_shared<ExpFunc<long double>>(
				std::make_shared<DivOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Constant<long double>>(6)
				)
			)->diff("x")->with_values({{"x", 6}})->resolve(),
			e_by_six
		),
		"d/dx e^(x/6) = 1/6 * e^(x/6)"
	);
	// clang-format on
	success &= tester_exp.run_tests();

	if (!success) throw std::logic_error("\033[1;31mSOME TESTS FAILED!\033[0m");
	std::cout << "\033[1;32m\nALL TESTS PASSED SUCCESSFULLY!\033[0m\n"
			  << std::endl;
	return 0;
}
