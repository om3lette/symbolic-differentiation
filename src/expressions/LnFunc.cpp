#include "expressions.hpp"

namespace Derivative {

template <typename T>
LnFunc<T>::LnFunc(std::shared_ptr<BaseExpression<T>> _argument)
	: argument(_argument){};

template <typename T>
std::shared_ptr<BaseExpression<T>> LnFunc<T>::diff(const std::string &by
) const {
	auto ln_diff = std::make_shared<DivOp<T>>(
		std::make_shared<Constant<T>>(1.0L), argument
	);
	return std::make_shared<MultOp<T>>(ln_diff, argument->diff(by));
}

template <typename T>
std::shared_ptr<BaseExpression<T>>
LnFunc<T>::with_values(const std::unordered_map<std::string, T> &values) const {
	return std::make_shared<LnFunc<T>>(argument->with_values(values));
};

template <typename T> T LnFunc<T>::resolve() const {
	T arg_value = argument->resolve();
	if (arg_value <= 0.0L)
		throw std::runtime_error("Ln of negative value in LnFunc::resolve");
	return std::log(arg_value);
};

template <>
std::complex<long double> LnFunc<std::complex<long double>>::resolve() const {
	throw std::runtime_error(
		"Logarithm of complex numbers is not supported in this implementation"
	);
}

template <typename T> std::string LnFunc<T>::to_string(void) const {
	return "ln(" + argument->to_string() + ")";
};

template <typename T>
std::shared_ptr<BaseExpression<T>> LnFunc<T>::prettify() const {
	std::shared_ptr<BaseExpression<T>> new_arg = argument->prettify();

	// clang-format off
	if (is_const(new_arg))
		return std::make_shared<Constant<T>>(std::log(new_arg->resolve()));
	// clang-format on
	return std::make_shared<LnFunc<T>>(new_arg);
}

template class LnFunc<long double>;
template class LnFunc<std::complex<long double>>;
} // namespace Derivative
