#pragma once
#include <functional>
#include <iostream>
#include <sstream>

using namespace Derivative;

template <typename T> std::function<bool()> is_equal(T x, T y) {
	return [x, y]() { return x == y; };
}

bool is_lexer_equal(Lexer lexer, const std::vector<Token> &answer) {
	for (auto token : answer) {
		if (!is_equal(lexer.next_token(), token)) return false;
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
