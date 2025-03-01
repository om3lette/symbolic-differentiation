#include "expressions.hpp"

namespace Derivative {

template <typename T>
MultOp<T>::MultOp(
	const std::shared_ptr<BaseExpression<T>> &_left,
	const std::shared_ptr<BaseExpression<T>> &_right
)
	: left(_left), right(_right) {}

template <typename T>
std::shared_ptr<BaseExpression<T>> MultOp<T>::diff(const std::string &by
) const {
	return std::make_shared<AddOp<T>>(
		std::make_shared<MultOp<T>>(left->diff(by), right),
		std::make_shared<MultOp<T>>(left, right->diff(by))
	);
};
template <typename T>
std::shared_ptr<BaseExpression<T>>
MultOp<T>::with_values(const std::unordered_map<std::string, T> &values) const {
	return std::make_shared<MultOp<T>>(
		left->with_values(values), right->with_values(values)
	);
};

template <typename T> T MultOp<T>::resolve() const {
	return left->resolve() * right->resolve();
};

template <typename T> std::string MultOp<T>::to_string(void) const {
	return "(" + left->to_string() + ") * (" + right->to_string() + ")";
};

template class MultOp<long double>;
template class MultOp<std::complex<long double>>;
} // namespace Derivative
