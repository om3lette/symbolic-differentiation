#include <memory>

namespace Derivative {

template <typename T> class BaseExpression;
template <typename T> class Constant;

template <typename T>
bool is_const(const std::shared_ptr<BaseExpression<T>> &expr) {
	if (auto constant = std::dynamic_pointer_cast<Constant<T>>(expr))
		return true;
	return false;
}

template <typename T>
bool is_zero(const std::shared_ptr<BaseExpression<T>> &expr) {
	if (auto constant = std::dynamic_pointer_cast<Constant<T>>(expr)) {
		try {
			return constant->resolve() == T(0);
		} catch (...) {
			return false;
		}
	}
	return false;
}

template <typename T>
bool is_one(const std::shared_ptr<BaseExpression<T>> &expr) {
	if (auto constant = std::dynamic_pointer_cast<Constant<T>>(expr)) {
		try {
			return constant->resolve() == T(1);
		} catch (...) {
			return false;
		}
	}
	return false;
}
} // namespace Derivative