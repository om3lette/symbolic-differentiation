#include "expressions.hpp"

namespace Derivative {

template <typename T> Constant<T>::Constant(T number) : value(number){};

template <typename T>
std::shared_ptr<BaseExpression<T>>
Constant<T>::diff([[maybe_unused]] const std::string &by) const {
	return std::make_shared<Constant<T>>(Constant<T>(0));
}

template <typename T>
std::shared_ptr<BaseExpression<T>> Constant<T>::with_values(
	[[maybe_unused]] const std::unordered_map<std::string, T> &values
) const {
	return std::make_shared<Constant<T>>(Constant<T>(value));
};

template <typename T> T Constant<T>::resolve() const { return value; };

template <typename T> std::string Constant<T>::to_string(void) const {
	std::ostringstream oss;
	oss << value;
	return oss.str();
};

template <>
std::string Constant<std::complex<long double>>::to_string(void) const {
	std::ostringstream oss;
	bool with_both_parts = false;
	if (value.imag() == 0 && value.real() == 0)
		return "0";
	else if (value.imag() != 0 && value.real() != 0) {
		with_both_parts = true;
		oss << "(";
	}
	if (value.real() != 0) oss << value.real();

	if (with_both_parts && value.imag() >= 0)
		oss << " + ";
	else if (with_both_parts)
		oss << " - ";

	if (value.imag() != 0) {
		if (std::abs(value.imag()) != 1) oss << std::abs(value.imag());
		oss << "i";
	}

	if (with_both_parts) oss << ")";
	return oss.str();
}

template class Constant<long double>;
template class Constant<std::complex<long double>>;
} // namespace Derivative
