#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <functional>
#include <iostream>
#include <sstream>

using namespace Derivative;

const long double e = std::exp(1.0L);
const long double pi = std::acos(-1.0L);

template <typename T> std::function<bool()> is_equal(T x, T y) {
	return [x, y]() { return x == y; };
}

template <typename T>
std::function<bool()> is_equal_complex(
	const std::complex<T> &a, const std::complex<T> &b, T epsilon = 1e-6
) {
	return [a, b, epsilon]() mutable {
		return std::abs(a.real() - b.real()) < epsilon &&
			   std::abs(a.imag() - b.imag()) < epsilon;
	};
}

template <typename T>
std::function<bool()> is_within_tolerance(T x, T y, T epsilon = 1e-6) {
	return [x, y, epsilon]() { return std::abs(x - y) < epsilon; };
}

bool is_token_equal(const Token &t1, const Token &t2) {
	return t1.type == t2.type && t1.value == t2.value;
}

bool is_lexer_equal(Lexer lexer, const std::vector<Token> &answer) {
	for (size_t i = 0; i < answer.size(); i++) {
		if (!is_token_equal(lexer.next_token(), answer[i])) return false;
	};
	// Only return true if all the Tokens coincided and the end was reached
	return lexer.next_token().type == TokenType::End;
}

std::function<void()> create_lexer_iterator(Lexer lexer) {
	return [lexer]() mutable {
		// No body intended
		while (lexer.next_token().type != TokenType::End);
	};
}

std::function<bool()>
compare_output(std::function<void()> func, const std::string &expectedValue) {
	return [func, expectedValue]() {
		std::stringstream buffer;
		std::streambuf *old = std::cout.rdbuf();
		std::cout.rdbuf(buffer.rdbuf());

		func();

		std::cout.rdbuf(old);
		return buffer.str() == expectedValue;
	};
}
#endif