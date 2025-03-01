#include "expressions.hpp"

namespace Derivative {
template <typename T> Expression<T>::Expression(T number) {
	value = std::shared_ptr<BaseExpression<T>>(Constant<T>(number));
};
template <typename T> Expression<T>::Expression(std::string &variable) {
	value = std::shared_ptr<BaseExpression<T>>(Variable<T>(variable));
};
template <typename T>
Expression<T>::Expression(std::shared_ptr<BaseExpression<T>> expression_impl) {
	value = expression_impl;
};

// template class Expression<long double>;
// template class Expression<std::complex<long double>>;
} // namespace Derivative