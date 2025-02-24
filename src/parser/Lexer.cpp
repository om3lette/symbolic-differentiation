#include "Lexer.hpp"
#include <algorithm>
#include <assert.h>
#include <cctype>
#include <iostream>
#include <stdexcept>

namespace Derivative {

Lexer::Lexer(const std::string &s) {
	input = Lexer::to_lower(s);
	pos = 0;
	add_mult = false;
};

void Lexer::advance(void) { pos++; }

char Lexer::peek(void) const {
	assert("Index out of range " && pos <= input.size());
	return input[pos];
}

std::string Lexer::to_lower(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

bool Lexer::is_reserved(const std::string &str) const {
	return reserved_words.find(str) != reserved_words.end();
}

Token Lexer::next_token(void) {
	if (add_mult) {
		add_mult = false;
		return Token(TokenType::Operator, "*");
	}

	// Skip until a non space symbol
	while (std::isspace(peek())) advance();

	if (peek() == '\0') return Token(TokenType::End, "\0");

	if (std::isdigit(peek())) {
		std::string number = "";
		while (std::isdigit(peek()) || peek() == '.') {
			number += peek();
			advance();
		};
		if (isalpha(peek())) add_mult = true;
		return Token(TokenType::Number, number);
	}

	if (std::isalpha(peek())) {
		std::string accumulator = "";
		while (std::isalpha(peek())) {
			accumulator += peek();
			advance();
		};
		if (peek() == '(')
			return Token(TokenType::Function, accumulator);
		else if (is_reserved(accumulator)) {
			throw std::invalid_argument(
				"Reserved sequence can not be used as variable name!"
			);
		}
		if (accumulator == "i") return Token(TokenType::Complex, accumulator);
		return Token(TokenType::Identifier, accumulator);
	};
	Token return_token;
	switch (peek()) {
	case '+':
	case '-':
	case '*':
	case '/':
	case '^':
		return_token = Token(TokenType::Operator, std::string(1, peek()));
		break;
	case '(':
		return_token = Token(TokenType::LeftParen, "(");
		break;
	case ')':
		return_token = Token(TokenType::RightParen, ")");
		break;
	default:
		throw std::invalid_argument("Unexpected character");
	}
	advance();
	return return_token;
}
} // namespace Derivative
