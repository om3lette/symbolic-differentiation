#include "expressions.hpp"
#include <memory>
#include <sstream>

namespace Derivative {

template <typename T>
SubOp<T>::SubOp(
	const std::shared_ptr<BaseExpression<T>> &_left,
	const std::shared_ptr<BaseExpression<T>> &_right
)
	: left(_left), right(_right) {}

template <typename T>
std::shared_ptr<BaseExpression<T>> SubOp<T>::diff(const std::string &by) const {
	return std::make_shared<SubOp<T>>(left->diff(by), right->diff(by));
}

template <typename T>
std::shared_ptr<BaseExpression<T>>
SubOp<T>::with_values(const std::unordered_map<std::string, T> &values) const {
	return std::make_shared<SubOp<T>>(
		left->with_values(values), right->with_values(values)
	);
}

template <typename T> T SubOp<T>::resolve() const {
	return left->resolve() - right->resolve();
}

template <typename T> std::string SubOp<T>::to_string() const {
	return "(" + left->to_string() + " - " + right->to_string() + ")";
}

template class SubOp<long double>;
template class SubOp<std::complex<long double>>;
} // namespace Derivative