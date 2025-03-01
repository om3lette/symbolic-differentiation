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
	bool add_mult;
	std::unordered_set<std::string> reserved_words = {
		"sin", "cos", "ln", "exp"
	};

	char peek(void) const;
	void advance(void);

	Token deduce_identifier(const std::string &value);

	bool is_reserved(const std::string &str) const;
	static std::string to_lower(std::string str);

  public:
	Lexer(const std::string &s);
	Token next_token(void);
};

} // namespace Derivative

#endif
