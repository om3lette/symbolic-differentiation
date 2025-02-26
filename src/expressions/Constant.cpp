#include "expressions.hpp"

namespace Derivative {
template <typename T> Constant<T>::Constant(T value) : value(value){};

template <typename T>
std::unique_ptr<Constant<T>>
Constant<T>::diff_impl([[maybe_unused]] const std::string &by) const {
	return std::make_unique<Constant<T>>(Constant<T>(0));
}

template <typename T>
std::unique_ptr<Constant<T>> Constant<T>::with_values_impl(
	[[maybe_unused]] const std::unordered_map<std::string, T> &values
) const {
	return std::make_unique<Constant<T>>(Constant<T>(value));
};

template <typename T>
std::unique_ptr<Constant<T>> Constant<T>::resolve_impl() const {
	return std::make_unique<Constant<T>>(value);
};

template <typename T> std::string Constant<T>::to_string_impl(void) const {
	return std::to_string(value);
};

template <>
std::string Constant<std::complex<long double>>::to_string_impl(void) const {
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

template <typename T> T Constant<T>::get_value_impl(void) const {
	return value;
};

template class Constant<long double>;
template class Constant<std::complex<long double>>;
} // namespace Derivative