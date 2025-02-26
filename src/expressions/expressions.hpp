#ifndef EXPRESSIONS_HPP
#define EXPRESSIONS_HPP

#include <complex>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace Derivative {
template <typename Derived, typename T> class BaseExpression {
  private:
	const Derived *get_expr() const {
		return static_cast<const Derived *>(this);
	}

  public:
	virtual ~BaseExpression() = default;

	std::unique_ptr<BaseExpression> diff(const std::string &by) const {
		static_assert(
			!std::is_same<T, std::complex<double>>::value,
			"Differentiation is not supported for std::complex."
		);
		return get_expr()->diff_impl(by);
	}

	std::unique_ptr<BaseExpression>
	with_values(const std::unordered_map<std::string, T> &values) const {
		return get_expr()->with_values_impl(values);
	}

	// Recalculate the expression tree
	std::unique_ptr<BaseExpression> resolve() const {
		return get_expr()->resolve_impl();
	}

	std::string to_string(void) const { return get_expr()->to_string_impl(); };
	T get_value(void) const { return get_expr()->get_value_impl(); };
};

template <typename T> class Constant : public BaseExpression<Constant<T>, T> {
  private:
	T value;

	friend class BaseExpression<Constant<T>, T>;

	std::unique_ptr<Constant<T>> diff_impl(const std::string &by) const;
	std::unique_ptr<Constant<T>>
	with_values_impl(const std::unordered_map<std::string, T> &values) const;
	std::unique_ptr<Constant<T>> resolve_impl(void) const;

	std::string to_string_impl(void) const;
	T get_value_impl(void) const;

  public:
	explicit Constant(T value);
};
} // namespace Derivative

#endif