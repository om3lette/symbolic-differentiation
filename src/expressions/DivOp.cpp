#include "expressions.hpp"

namespace Derivative {

template <typename T>
DivOp<T>::DivOp(
	const std::shared_ptr<BaseExpression<T>> &_left,
	const std::shared_ptr<BaseExpression<T>> &_right
)
	: left(_left), right(_right) {}

template <typename T>
std::shared_ptr<BaseExpression<T>> DivOp<T>::diff(const std::string &by) const {
	auto numerator = std::make_shared<SubOp<T>>(
		std::make_shared<MultOp<T>>(left->diff(by), right),
		std::make_shared<MultOp<T>>(left, right->diff(by))
	);
	auto denominator =
		std::make_shared<PowOp<T>>(right, std::make_shared<Constant<T>>(2.0));

	return std::make_shared<DivOp<T>>(numerator, denominator);
};
template <typename T>
std::shared_ptr<BaseExpression<T>>
DivOp<T>::with_values(const std::unordered_map<std::string, T> &values) const {
	return std::make_shared<DivOp<T>>(
		DivOp(left->with_values(values), right->with_values(values))
	);
};

template <typename T> T DivOp<T>::resolve() const {
	T right_value = right->resolve();
	if (right_value == 0.0L)
		throw std::runtime_error("Division by zero in DivOp::resolve");
	return left->resolve() / right_value;
};

template <typename T> std::string DivOp<T>::to_string(void) const {
	return "(" + left->to_string() + ") / (" + right->to_string() + ")";
};

template class DivOp<long double>;
template class DivOp<std::complex<long double>>;
} // namespace Derivative
