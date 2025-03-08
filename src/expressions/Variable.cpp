#include "../string_utils.hpp"
#include "expressions.hpp"

namespace Derivative {

template <typename T>
Variable<T>::Variable(std::string var) : value(to_lower(var)){};

template <typename T>
std::shared_ptr<BaseExpression<T>> Variable<T>::diff(const std::string &by
) const {
	if (by == value) {
		return std::make_shared<Constant<T>>(Constant<T>(1));
	}
	return std::make_shared<Constant<T>>(Constant<T>(0));
}

template <typename T>
std::shared_ptr<BaseExpression<T>>
Variable<T>::with_values(const std::unordered_map<std::string, T> &values
) const {
	if (values.find(value) == values.end())
		return std::make_shared<Variable<T>>(Variable<T>(value));
	return std::make_shared<Constant<T>>(Constant<T>(values.at(value)));
};

template <typename T> T Variable<T>::resolve() const {
	throw std::runtime_error("Variable cannot be resolved: " + value);
};

template <typename T> std::string Variable<T>::to_string(void) const {
	return value;
};

template <typename T>
std::shared_ptr<BaseExpression<T>> Variable<T>::prettify() const {
	return std::make_shared<Variable<T>>(value);
}

template class Variable<long double>;
template class Variable<std::complex<long double>>;
} // namespace Derivative
