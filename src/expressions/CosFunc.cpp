#include "expressions.hpp"

namespace Derivative {

template <typename T>
CosFunc<T>::CosFunc(std::shared_ptr<BaseExpression<T>> _argument)
	: argument(_argument){};

template <typename T>
std::shared_ptr<BaseExpression<T>> CosFunc<T>::diff(const std::string &by
) const {
	auto cos_diff = std::make_shared<MultOp<T>>(
		std::make_shared<Constant<T>>(-1.0L),
		std::make_shared<SinFunc<T>>(argument)
	);
	return std::make_shared<MultOp<T>>(cos_diff, argument->diff(by));
};

template <typename T>
std::shared_ptr<BaseExpression<T>>
CosFunc<T>::with_values(const std::unordered_map<std::string, T> &values
) const {
	return std::make_shared<CosFunc<T>>(argument->with_values(values));
};

template <typename T> T CosFunc<T>::resolve() const {
	return std::cos(argument->resolve());
};

template <typename T> std::string CosFunc<T>::to_string(void) const {
	return "cos(" + argument->to_string() + ")";
};

template class CosFunc<long double>;
template class CosFunc<std::complex<long double>>;
} // namespace Derivative
