#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <unordered_set>

namespace Derivative {

enum TokenType {
	Number,		// Numeric literal
	Complex,	// Complex literal
	Identifier, // Variable
	Operator,	// Aritmetics operators (+, -, *, /, ^)
	LeftParen,	// Left parenthesis '('
	RightParen, // Right parenthesis ')'
	Function,	// Function name (sin, cos, etc)
	EulerConst,
	End
};

struct Token {
	TokenType type;
	std::string value;
};

class Lexer {
  private:
	std::string input;
	size_t pos;
	long parenthesis_cnt;

	bool add_mult;
	TokenType prev_token_type;

	std::unordered_set<std::string> reserved_words = {
		"sin", "cos", "ln", "exp"
	};

	char peek(void) const;
	void advance(void);

	Token deduce_identifier(const std::string &value);

	bool is_reserved(const std::string &str) const;
	bool is_var_suitable(char c) const;
	Token push_mult(void);

  public:
	Lexer(const std::string &s);
	Token next_token(void);
};

} // namespace Derivative

#endif
