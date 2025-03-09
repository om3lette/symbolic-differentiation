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
	if (is_const(right))
		return "(" + left->to_string() + ") / " + right->to_string();
	if (is_const(left))
		return left->to_string() + " / (" + right->to_string() + ")";
	return "(" + left->to_string() + ") / (" + right->to_string() + ")";
};

template <typename T>
std::shared_ptr<BaseExpression<T>> DivOp<T>::prettify() const {
	auto new_left = left->prettify();
	auto new_right = right->prettify();

	if (is_one(new_right)) return new_left;
	if (is_zero(new_left)) return std::make_shared<Constant<T>>(0.0L);
	if (is_const(new_left) && is_const(new_right))
		return std::make_shared<Constant<T>>(
			new_left->resolve() / new_right->resolve()
		);

	return std::make_shared<DivOp<T>>(new_left, new_right);
}

template class DivOp<long double>;
template class DivOp<std::complex<long double>>;
} // namespace Derivative
