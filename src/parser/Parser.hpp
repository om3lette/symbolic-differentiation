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
	std::shared_ptr<BaseExpression<T>> create_function(
		const std::string &func_name,
		std::shared_ptr<BaseExpression<T>> argument
	);

	std::shared_ptr<BaseExpression<T>> parse_expression();
	std::shared_ptr<BaseExpression<T>> parse_term();
	std::shared_ptr<BaseExpression<T>> parse_factor();
	std::shared_ptr<BaseExpression<T>> parse_power();
	std::shared_ptr<Constant<T>> parse_complex();
};

} // namespace Derivative

#endif // PARSER_HPP
