#include "expressions.hpp"

namespace Derivative {

template <typename T>
SinFunc<T>::SinFunc(std::shared_ptr<BaseExpression<T>> _argument)
	: argument(_argument){};

template <typename T>
std::shared_ptr<BaseExpression<T>> SinFunc<T>::diff(const std::string &by
) const {
	auto sin_diff = std::make_shared<CosFunc<T>>(argument);
	return std::make_shared<MultOp<T>>(sin_diff, argument->diff(by));
};

template <typename T>
std::shared_ptr<BaseExpression<T>>
SinFunc<T>::with_values(const std::unordered_map<std::string, T> &values
) const {
	return std::make_shared<SinFunc<T>>(argument->with_values(values));
};

template <typename T> T SinFunc<T>::resolve() const {
	return std::sin(argument->resolve());
};

template <typename T> std::string SinFunc<T>::to_string(void) const {
	return "sin(" + argument->to_string() + ")";
};

template class SinFunc<long double>;
template class SinFunc<std::complex<long double>>;
} // namespace Derivative
