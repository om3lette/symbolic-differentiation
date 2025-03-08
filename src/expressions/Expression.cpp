#include "../parser/Parser.hpp"
#include "expressions.hpp"

namespace Derivative {

template <typename T>
Expression<T>::Expression(T number)
	: value(std::make_shared<Constant<T>>(number)){};

template <typename T>
Expression<T>::Expression(const std::string &variable)
	: value(std::make_shared<Variable<T>>(variable)){};

template <typename T>
Expression<T>::Expression(std::shared_ptr<BaseExpression<T>> expression_impl)
	: value(expression_impl) {}

template <typename T>
Expression<T> Expression<T>::from_string(const std::string &expression_string) {
	return Parser<T>(expression_string).parse();
}

template <typename T>
Expression<T> &Expression<T>::operator=(Expression &&other) {
	if (this != &other) {
		value = std::move(other.value);
	}
	return *this;
}

template <typename T>
Expression<T> &Expression<T>::operator=(const Expression &other) {
	if (this != &other) {
		value = other.value;
	}
	return *this;
}

template <typename T>
Expression<T>::Expression(const Expression &other) : value(other.value) {}

template <typename T>
Expression<T> Expression<T>::operator+(const Expression &other) const {
	return Expression<T>(std::make_shared<AddOp<T>>(value, other.value));
};
template <typename T>
Expression<T> &Expression<T>::operator+=(const Expression &other) {
	*this = *this + other;
	return *this;
};

template <typename T>
Expression<T> Expression<T>::operator-(const Expression &other) const {
	return Expression<T>(std::make_shared<SubOp<T>>(value, other.value));
};
template <typename T>
Expression<T> &Expression<T>::operator-=(const Expression &other) {
	*this = *this - other;
	return *this;
};

template <typename T>
Expression<T> Expression<T>::operator*(const Expression &other) const {
	return Expression<T>(std::make_shared<MultOp<T>>(value, other.value));
};
template <typename T>
Expression<T> &Expression<T>::operator*=(const Expression &other) {
	*this = *this * other;
	return *this;
};

template <typename T>
Expression<T> Expression<T>::operator/(const Expression &other) const {
	return Expression<T>(std::make_shared<DivOp<T>>(value, other.value));
};
template <typename T>
Expression<T> &Expression<T>::operator/=(const Expression &other) {
	*this = *this / other;
	return *this;
};

template <typename T>
Expression<T> Expression<T>::operator^(const Expression &other) const {
	return Expression<T>(std::make_shared<PowOp<T>>(value, other.value));
};
template <typename T>
Expression<T> &Expression<T>::operator^=(const Expression &other) {
	*this = *this ^ other;
	return *this;
};

template <typename T> Expression<T> Expression<T>::sin(void) const {
	return Expression<T>(std::make_shared<SinFunc<T>>(value));
};

template <typename T> Expression<T> Expression<T>::cos(void) const {
	return Expression<T>(std::make_shared<CosFunc<T>>(value));
};

template <typename T> Expression<T> Expression<T>::ln(void) const {
	return Expression<T>(std::make_shared<LnFunc<T>>(value));
};

template <typename T> Expression<T> Expression<T>::exp(void) const {
	return Expression<T>(std::make_shared<ExpFunc<T>>(value));
};

template <typename T>
Expression<T> Expression<T>::diff(const std::string &by) const {
	return Expression<T>(value->diff(by));
}

template <typename T>
Expression<T>
Expression<T>::with_values(const std::unordered_map<std::string, T> &values
) const {
	return Expression<T>(value->with_values(values));
}

template <typename T> T Expression<T>::resolve(void) const {
	return value->resolve();
}

template <typename T>
T Expression<T>::resolve_with(const std::unordered_map<std::string, T> &values
) const {
	return value->with_values(values)->resolve();
}

template <typename T> std::string Expression<T>::to_string(void) const {
	return value->to_string();
}

template <typename T> Expression<T> Expression<T>::prettify() const {
	return Expression<T>(value->prettify());
}

template class Expression<long double>;
template class Expression<std::complex<long double>>;
} // namespace Derivative