#include "expressions.hpp"

namespace Derivative {

template <typename T>
PowOp<T>::PowOp(
	const std::shared_ptr<BaseExpression<T>> &_left,
	const std::shared_ptr<BaseExpression<T>> &_right
)
	: left(_left), right(_right) {}

template <typename T>
std::shared_ptr<BaseExpression<T>> PowOp<T>::diff(const std::string &by) const {
	// left^right * (right' * ln(left) + (right * left') / left)

	// clang-format off
	// right' * ln(left)
	auto term1 = std::make_shared<MultOp<T>>(
		right->diff(by),
		std::make_shared<LnFunc<T>>(left)
	);

	// (right * left') / left
	auto term2 = std::make_shared<DivOp<T>>(
		std::make_shared<MultOp<T>>(right,left->diff(by)),
		left
	);
	// clang-format on
	// left^right * (term1 + term2)
	return std::make_shared<MultOp<T>>(
		std::make_shared<PowOp<T>>(left, right),
		std::make_shared<AddOp<T>>(term1, term2)
	);
};
template <typename T>
std::shared_ptr<BaseExpression<T>>
PowOp<T>::with_values(const std::unordered_map<std::string, T> &values) const {
	return std::make_shared<PowOp<T>>(
		left->with_values(values), right->with_values(values)
	);
};

template <typename T> T PowOp<T>::resolve() const {
	return std::pow(left->resolve(), right->resolve());
};

template <typename T> std::string PowOp<T>::to_string(void) const {
	return "(" + left->to_string() + ") ^ (" + right->to_string() + ")";
};

template class PowOp<long double>;
template class PowOp<std::complex<long double>>;
} // namespace Derivative
