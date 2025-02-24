#ifndef PARSER_HPP
#define PARSER_HPP

#include "Lexer.hpp"
#include <memory>
#include <stdexcept>

class Expression;

namespace Derivative {
class Parser {
  private:
	Lexer &lexer;
	Token current_token;

	void consume(TokenType expected);

	std::unique_ptr<Expression> parsePrimary();
	std::unique_ptr<Expression> parseFactor();
	std::unique_ptr<Expression> parseTerm();
	std::unique_ptr<Expression> parseExpr();

  public:
	Parser(Lexer &lexer);
	std::unique_ptr<Expression> parse();
};
} // namespace Derivative

#endif