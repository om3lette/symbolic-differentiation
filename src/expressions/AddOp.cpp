#include "expressions.hpp"
#include <memory>
#include <sstream>

namespace Derivative {

template <typename T>
AddOp<T>::AddOp(
	const std::shared_ptr<BaseExpression<T>> &_left,
	const std::shared_ptr<BaseExpression<T>> &_right
)
	: left(_left), right(_right) {}

template <typename T>
std::shared_ptr<BaseExpression<T>> AddOp<T>::diff(const std::string &by) const {
	return std::make_shared<AddOp<T>>(left->diff(by), right->diff(by));
}

template <typename T>
std::shared_ptr<BaseExpression<T>>
AddOp<T>::with_values(const std::unordered_map<std::string, T> &values) const {
	return std::make_shared<AddOp<T>>(
		left->with_values(values), right->with_values(values)
	);
}

template <typename T> T AddOp<T>::resolve() const {
	return left->resolve() + right->resolve();
}

template <typename T> std::string AddOp<T>::to_string() const {
	return "(" + left->to_string() + " + " + right->to_string() + ")";
}

template class AddOp<long double>;
template class AddOp<std::complex<long double>>;
} // namespace Derivative