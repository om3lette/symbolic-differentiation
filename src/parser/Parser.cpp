#include "Parser.hpp"
#include "../expressions/expressions.hpp"
#include <complex>
#include <memory>
#include <stdexcept>
#include <type_traits>

namespace Derivative {

// Returns the respective function `std::shared_ptr<BaseExpression<T>>`
// If no match for `func_name` is found match throws `std::runtime_error`
template <typename T>
std::shared_ptr<BaseExpression<T>> Parser<T>::create_function(
	const std::string &func_name, std::shared_ptr<BaseExpression<T>> argument
) {
	if (func_name == "sin") return std::make_shared<SinFunc<T>>(argument);
	if (func_name == "cos") return std::make_shared<CosFunc<T>>(argument);
	if (func_name == "ln") return std::make_shared<LnFunc<T>>(argument);
	if (func_name == "exp") return std::make_shared<ExpFunc<T>>(argument);
	throw std::runtime_error("Unknown function: " + func_name);
}

// Advances to the next token
// Throws `std::runtime_error` if expected Tokentype doesnt match current
template <typename T> void Parser<T>::consume(TokenType expected) {
	if (current_token.type != expected)
		throw std::runtime_error("Unexpected token: " + current_token.value);
	current_token = lexer.next_token();
};

// Parse a factor (number, variable, function call, or parenthesized expression)
template <typename T>
std::shared_ptr<BaseExpression<T>> Parser<T>::parse_factor() {
	Token token = current_token;

	// Handle unary minus (e.g -4)
	if (token.type == TokenType::Operator && token.value == "-") {
		consume(TokenType::Operator);
		auto factor = parse_factor();
		return std::make_shared<MultOp<T>>(
			std::make_shared<Constant<T>>(T(-1)), factor
		);
	}

	switch (token.type) {
	case TokenType::Number:
		consume(TokenType::Number);
		return std::make_shared<Constant<T>>(std::stold(token.value));

	case TokenType::EulerConst: {
		consume(TokenType::EulerConst); // Consume 'e'

		// Handle e as const ≈ 2.71828
		if (current_token.type != TokenType::Operator ||
			current_token.value != "^") {
			return std::make_shared<Constant<T>>(std::exp(1.0));
		}
		// Handle e^... as exp(...)
		consume(TokenType::Operator); // Consume '^'
		auto exponent = parse_power();
		return std::make_shared<ExpFunc<T>>(exponent);
	}

	case TokenType::Identifier: {
		consume(TokenType::Identifier);
		return std::make_shared<Variable<T>>(token.value);
	}

	case TokenType::LeftParen: {
		consume(TokenType::LeftParen);
		auto result = parse_expression();
		consume(TokenType::RightParen);
		return result;
	}

	case TokenType::Function: {
		consume(TokenType::Function);
		consume(TokenType::LeftParen);
		auto arg = parse_expression();
		consume(TokenType::RightParen);
		return create_function(token.value, arg);
	}

	case TokenType::Complex: {
		consume(TokenType::Complex);
		return parse_complex();
	}

	default:
		// The rest of TokenType should be handled elsewhere
		throw std::runtime_error(
			"Unexpected token in factor: " + current_token.value
		);
	}
}

template <typename T> std::shared_ptr<Constant<T>> Parser<T>::parse_complex() {
	throw std::runtime_error("Imaginary number found in expression. Please use "
							 "Parser<std::complex<long double>> instead");
}

template <>
std::shared_ptr<Constant<std::complex<long double>>>
Parser<std::complex<long double>>::parse_complex() {
	std::complex<long double> complex_value(0.0L, 1.0L);
	return std::make_shared<Constant<std::complex<long double>>>(complex_value);
}

// Parse a term (multiplication and division)
template <typename T>
std::shared_ptr<BaseExpression<T>> Parser<T>::parse_term() {
	auto result =
		parse_power(); // Start with parse_power() to handle exponentiation
	while (current_token.type == TokenType::Operator &&
		   (current_token.value == "*" || current_token.value == "/")) {
		Token token = current_token;

		consume(TokenType::Operator); // Consume the operator

		if (token.value == "*") {
			result = std::make_shared<MultOp<T>>(result, parse_power());
		} else if (token.value == "/") {
			result = std::make_shared<DivOp<T>>(result, parse_power());
		}
	}
	return result;
};

// Parse an expression (addition and subtraction)
template <typename T>
std::shared_ptr<BaseExpression<T>> Parser<T>::parse_expression() {
	auto result = parse_term();
	while (current_token.type == TokenType::Operator &&
		   (current_token.value == "+" || current_token.value == "-")) {
		Token token = current_token;

		consume(TokenType::Operator);

		if (token.value == "+") {
			result = std::make_shared<AddOp<T>>(result, parse_term());
		} else if (token.value == "-") {
			result = std::make_shared<SubOp<T>>(result, parse_term());
		}
	}
	return result;
};

// Parse a power (exponentiation)
template <typename T>
std::shared_ptr<BaseExpression<T>> Parser<T>::parse_power() {
	auto result = parse_factor(); // Start with factor
	while (current_token.type == TokenType::Operator &&
		   current_token.value == "^") {

		consume(TokenType::Operator); // Consume '^'

		result = std::make_shared<PowOp<T>>(result, parse_factor());
	}
	return result;
};

template <typename T>
Parser<T>::Parser(const std::string &expression_str) : lexer(expression_str) {
	current_token = lexer.next_token();
}

// Parse the input and return the resulting expression
template <typename T> Expression<T> Parser<T>::parse() {
	return Expression<T>(parse_expression());
}

template class Parser<long double>;
template class Parser<std::complex<long double>>;
} // namespace Derivative
