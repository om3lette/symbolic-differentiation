#include "Parser.hpp"
#include "../expressions/expressions.hpp"
#include <complex>
#include <memory>
#include <stdexcept>
#include <type_traits>

namespace Derivative {

template <typename T> void Parser<T>::consume(TokenType expected) {
	if (current_token.type != expected)
		throw std::runtime_error("Unexpected token: " + current_token.value);
	current_token = lexer.next_token();
};

// Parse a factor (number, variable, function call, or parenthesized expression)
template <typename T>
std::shared_ptr<BaseExpression<T>> Parser<T>::parseFactor() {
	Token token = current_token;

	if (token.type == TokenType::Operator && token.value == "-") {
		consume(TokenType::Operator);
		auto factor = parseFactor();
		return std::make_shared<MultOp<T>>(
			std::make_shared<Constant<T>>(T(-1)), factor
		);
	}
	if (token.type == TokenType::Number) {
		consume(TokenType::Number);
		return std::make_shared<Constant<T>>(std::stold(token.value));
	} else if (token.type == TokenType::EulerConst) {
		// Handle e^... as exp(...)
		consume(TokenType::EulerConst); // Consume 'e'
		if (current_token.type == TokenType::Operator &&
			current_token.value == "^") {
			consume(TokenType::Operator); // Consume '^'
			// Parse the exponent using parsePower()
			auto exponent = parsePower();
			return std::make_shared<ExpFunc<T>>(exponent);
		} else {
			// If it's just 'e' without '^', trconsume it as the constant e
			// e ≈ 2.71828
			return std::make_shared<Constant<T>>(std::exp(1.0));
		}
	} else if (token.type == TokenType::Identifier) {
		consume(TokenType::Identifier);
		return std::make_shared<Variable<T>>(token.value);
	} else if (token.type == TokenType::LeftParen) {
		consume(TokenType::LeftParen);
		auto result = parseExpression();
		consume(TokenType::RightParen);
		return result;
	} else if (token.type == TokenType::Function) {
		std::string func_name = token.value;
		consume(TokenType::Function);
		consume(TokenType::LeftParen);
		auto arg = parseExpression();
		consume(TokenType::RightParen);
		if (func_name == "sin") {
			return std::make_shared<SinFunc<T>>(arg);
		} else if (func_name == "cos") {
			return std::make_shared<CosFunc<T>>(arg);
		} else if (func_name == "ln") {
			return std::make_shared<LnFunc<T>>(arg);
		} else if (func_name == "exp") {
			return std::make_shared<ExpFunc<T>>(arg);
		} else {
			throw std::runtime_error("Unknown function");
		}
	} else if (token.type == TokenType::Complex) {
		consume(TokenType::Complex);
		return parse_complex();
	} else {
		throw std::runtime_error(
			"Unexpected token in factor: " + current_token.value
		);
	}
};

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
std::shared_ptr<BaseExpression<T>> Parser<T>::parseTerm() {
	auto result =
		parsePower(); // Start with parsePower() to handle exponentiation
	while (current_token.type == TokenType::Operator &&
		   (current_token.value == "*" || current_token.value == "/")) {
		Token token = current_token;
		consume(TokenType::Operator); // Consume the operator
		if (token.value == "*") {
			result = std::make_shared<MultOp<T>>(result, parsePower());
		} else if (token.value == "/") {
			result = std::make_shared<DivOp<T>>(result, parsePower());
		}
	}
	return result;
};

// Parse an expression (addition and subtraction)
template <typename T>
std::shared_ptr<BaseExpression<T>> Parser<T>::parseExpression() {
	auto result = parseTerm();
	while (current_token.type == TokenType::Operator &&
		   (current_token.value == "+" || current_token.value == "-")) {
		Token token = current_token;
		consume(TokenType::Operator);
		if (token.value == "+") {
			result = std::make_shared<AddOp<T>>(result, parseTerm());
		} else if (token.value == "-") {
			result = std::make_shared<SubOp<T>>(result, parseTerm());
		}
	}
	return result;
};

// Parse a parsePower() (exponentiation)
template <typename T>
std::shared_ptr<BaseExpression<T>> Parser<T>::parsePower() {
	auto result = parseFactor(); // Start with factor
	while (current_token.type == TokenType::Operator &&
		   current_token.value == "^") {
		Token token = current_token;
		consume(TokenType::Operator); // Consume '^'
		result = std::make_shared<PowOp<T>>(result, parseFactor());
	}
	return result;
};

template <typename T>
Parser<T>::Parser(const std::string &expression_str) : lexer(expression_str) {
	current_token = lexer.next_token();
}

// Parse the input and return the resulting expression
template <typename T> Expression<T> Parser<T>::parse() {
	return Expression<T>(parseExpression());
}

template class Parser<long double>;
template class Parser<std::complex<long double>>;
} // namespace Derivative
