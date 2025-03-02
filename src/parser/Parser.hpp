#ifndef PARSER_HPP
#define PARSER_HPP

#include "../expressions/expressions.hpp"
#include "Lexer.hpp"
#include <memory>

namespace Derivative {

template <typename T> class Parser {
  public:
	explicit Parser(const std::string &expression_str);

	Expression<T> parse();

  private:
	Lexer lexer;
	Token current_token;

	void consume(TokenType expected);
	std::shared_ptr<BaseExpression<T>> parseExpression();
	std::shared_ptr<BaseExpression<T>> parseTerm();
	std::shared_ptr<BaseExpression<T>> parseFactor();
	std::shared_ptr<BaseExpression<T>> parsePower();
	std::shared_ptr<Constant<T>> parse_complex();
};

} // namespace Derivative

#endif // PARSER_HPP
