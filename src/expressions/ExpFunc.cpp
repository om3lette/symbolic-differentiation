#include "expressions.hpp"

namespace Derivative {

template <typename T>
ExpFunc<T>::ExpFunc(std::shared_ptr<BaseExpression<T>> _argument)
	: argument(_argument){};

template <typename T>
std::shared_ptr<BaseExpression<T>> ExpFunc<T>::diff(const std::string &by
) const {
	auto e_diff = std::make_shared<ExpFunc<T>>(argument);
	return std::make_shared<MultOp<T>>(e_diff, argument->diff(by));
};

template <typename T>
std::shared_ptr<BaseExpression<T>>
ExpFunc<T>::with_values(const std::unordered_map<std::string, T> &values
) const {
	return std::make_shared<ExpFunc<T>>(argument->with_values(values));
};

template <typename T> T ExpFunc<T>::resolve() const {
	return std::exp(argument->resolve());
};

template <typename T> std::string ExpFunc<T>::to_string(void) const {
	return "e^(" + argument->to_string() + ")";
};

template <typename T>
std::shared_ptr<BaseExpression<T>> ExpFunc<T>::prettify() const {
	std::shared_ptr<BaseExpression<T>> new_arg = argument->prettify();

	// clang-format off
	if (is_const(new_arg))
		return std::make_shared<Constant<T>>(std::exp(new_arg->resolve()));
	// clang-format on
	return std::make_shared<ExpFunc<T>>(new_arg);
}

template class ExpFunc<long double>;
template class ExpFunc<std::complex<long double>>;
} // namespace Derivative
