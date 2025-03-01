#include "Lexer.hpp"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <iostream>
#include <stdexcept>

namespace Derivative {

Lexer::Lexer(const std::string &s) {
	input = Lexer::to_lower(s);
	pos = 0;
	add_mult = false;
	parenthesis_cnt = 0;
	prev_token_type = TokenType::End;
}

void Lexer::advance() { pos++; }

char Lexer::peek() const {
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

Token Lexer::deduce_identifier(const std::string &value) {
	if (prev_token_type != TokenType::End &&
		prev_token_type != TokenType::LeftParen &&
		prev_token_type != TokenType::Operator)
		throw std::runtime_error("Incorrect syntax in Lexer::next_token.\n "
								 "Operator is missing or parenthesis");
	if (value == "i") {
		prev_token_type = TokenType::Complex;
		return Token(TokenType::Complex, "i");
	}
	if (value == "e") {
		prev_token_type = TokenType::EulerConst;
		return Token(TokenType::EulerConst, "e");
	}
	prev_token_type = TokenType::Identifier;
	return Token(TokenType::Identifier, value);
}

Token Lexer::next_token() {
	if (add_mult) {
		add_mult = false;
		prev_token_type = TokenType::Operator;
		return Token(TokenType::Operator, "*");
	}

	// Skip whitespace
	while (std::isspace(peek())) advance();

	if (peek() == '\0') {
		if (parenthesis_cnt != 0)
			throw std::runtime_error(
				"Unmatched parenthesis in Lexer::next_token"
			);
		return Token(TokenType::End, "\0");
	}

	// Handle numbers
	if (std::isdigit(peek())) {
		std::string number = "";
		while (std::isdigit(peek()) || peek() == '.') {
			number += peek();
			advance();
		}
		if (std::isalpha(peek()) || peek() == '(') add_mult = true;
		prev_token_type = TokenType::Number;
		return Token(TokenType::Number, number);
	}

	// Handle identifiers and reserved words
	if (std::isalpha(peek())) {
		std::string accumulator = "";
		while (std::isalpha(peek())) {
			accumulator += peek();
			advance();
		}
		if (peek() == '(') {
			if (!is_reserved(accumulator)) {
				// Account for variables before parenthesis e.g "x(2 + x)"
				add_mult = true;
				return deduce_identifier(accumulator);
			}
			prev_token_type = TokenType::Function;
			return Token(TokenType::Function, accumulator);
		} else if (is_reserved(accumulator)) {
			throw std::invalid_argument(
				"Reserved sequence cannot be used as a variable name!"
			);
		}
		return deduce_identifier(accumulator);
	}

	// Handle operators and parentheses
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
		// clang-format off
		if (prev_token_type != TokenType::Operator &&
			prev_token_type != TokenType::Function &&
			prev_token_type != TokenType::End)
			throw std::runtime_error(
				"Incorrect syntax in Lexer::next_token.\nParenthesis must go "
				"after function/operator/variable/constant "
			);
		// clang-format on
		return_token = Token(TokenType::LeftParen, "(");
		parenthesis_cnt++;
		break;
	case ')':
		parenthesis_cnt--;
		return_token = Token(TokenType::RightParen, ")");
		break;
	default:
		throw std::invalid_argument("Unexpected character");
	}
	advance();

	// Handle implicit multiplication after a right parenthesis
	if (return_token.type == TokenType::RightParen &&
		(std::isalpha(peek()) || std::isdigit(peek()))) {
		add_mult = true;
	}

	prev_token_type = return_token.type;
	return return_token;
}

} // namespace Derivative