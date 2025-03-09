#include <memory>

namespace Derivative {
// Forward declaration
template <typename T> class BaseExpression;
template <typename T> class Constant;
template <typename T> class Variable;

template <typename T>
bool is_const(const std::shared_ptr<BaseExpression<T>> &expr) {
	return std::dynamic_pointer_cast<Constant<T>>(expr) != nullptr;
}

template <typename T>
bool is_var(const std::shared_ptr<BaseExpression<T>> &expr) {
	return std::dynamic_pointer_cast<Variable<T>>(expr) != nullptr;
}

template <typename T>
bool is_zero(const std::shared_ptr<BaseExpression<T>> &expr) {
	if (auto constant = std::dynamic_pointer_cast<Constant<T>>(expr)) {
		return constant->resolve() == T(0);
	}
	return false;
}

template <typename T>
bool is_one(const std::shared_ptr<BaseExpression<T>> &expr) {
	if (auto constant = std::dynamic_pointer_cast<Constant<T>>(expr)) {
		return constant->resolve() == T(1);
	}
	return false;
}
} // namespace Derivative