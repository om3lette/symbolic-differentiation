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
	if (is_const(left) && is_var(right))
		return left->to_string() + right->to_string();
	if (is_const(right) && is_var(left))
		return right->to_string() + left->to_string();
	return "(" + left->to_string() + ") * (" + right->to_string() + ")";
};

template <typename T>
std::shared_ptr<BaseExpression<T>> MultOp<T>::prettify() const {
	auto new_left = left->prettify();
	auto new_right = right->prettify();

	if (is_one(new_left)) return new_right;
	if (is_one(new_right)) return new_left;
	if (is_zero(new_left) || is_zero(new_right))
		return std::make_shared<Constant<T>>(0.0L);
	if (is_const(new_left) && is_const(new_right))
		return std::make_shared<Constant<T>>(
			new_left->resolve() * new_right->resolve()
		);

	return std::make_shared<MultOp<T>>(new_left, new_right);
}

template class MultOp<long double>;
template class MultOp<std::complex<long double>>;
} // namespace Derivative
