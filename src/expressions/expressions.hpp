#ifndef EXPRESSIONS_HPP
#define EXPRESSIONS_HPP

#include "expressions_utils.hpp"

#include <complex>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace Derivative {

// Forward declaration of Parser to use as friend of Expression
template <typename T> class Parser;

// FIXME
// template <typename T>
// using std::unordered_map<std::string, T> = const std::unordered_map<std::string, T>;

template <typename T> class BaseExpression {
  public:
	virtual ~BaseExpression() = default;

	virtual std::shared_ptr<BaseExpression<T>> diff(const std::string &by
	) const = 0;
	virtual std::shared_ptr<BaseExpression<T>>
	with_values(const std::unordered_map<std::string, T> &values) const = 0;
	virtual std::shared_ptr<BaseExpression<T>> prettify(void) const = 0;
	virtual T resolve(void) const = 0;
	virtual std::string to_string(void) const = 0;
};

template <typename T> class Expression {
  public:
	Expression(T number);
	Expression(const std::string &variable);
	static Expression<T> from_string(const std::string &expression_string);

	~Expression() = default;
	Expression(const Expression &other);

	Expression<T> sin(void) const;
	Expression<T> cos(void) const;
	Expression<T> ln(void) const;
	Expression<T> exp(void) const;

	Expression<T> diff(const std::string &by) const;
	Expression<T> with_values(const std::unordered_map<std::string, T> &values
	) const;
	T resolve(void) const;
	T resolve_with(const std::unordered_map<std::string, T> &values) const;
	std::string to_string(void) const;
	Expression<T> prettify(void) const;

	Expression<T> &operator=(const Expression<T> &other);
	Expression<T> &operator=(Expression<T> &&other);

	Expression<T> operator+(const Expression<T> &other) const;
	Expression<T> &operator+=(const Expression<T> &other);

	Expression<T> operator-(const Expression<T> &other) const;
	Expression<T> &operator-=(const Expression<T> &other);

	Expression<T> operator*(const Expression<T> &other) const;
	Expression<T> &operator*=(const Expression<T> &other);

	Expression<T> operator/(const Expression<T> &other) const;
	Expression<T> &operator/=(const Expression<T> &other);

	Expression<T> operator^(const Expression<T> &other) const;
	Expression<T> &operator^=(const Expression<T> &other);

  private:
	Expression(std::shared_ptr<BaseExpression<T>> expression_impl);
	std::shared_ptr<BaseExpression<T>> value;
	// Used to gain access to private constructor
	friend class Parser<T>;
};

template <typename T> class Constant : public BaseExpression<T> {
  private:
	T value;

  public:
	explicit Constant(T number);

	virtual std::shared_ptr<BaseExpression<T>> diff(const std::string &by
	) const override;
	virtual std::shared_ptr<BaseExpression<T>>
	with_values(const std::unordered_map<std::string, T> &values
	) const override;
	virtual T resolve(void) const override;
	virtual std::string to_string(void) const override;
	virtual std::shared_ptr<BaseExpression<T>> prettify(void) const override;
};

template <typename T> class Variable : public BaseExpression<T> {
  private:
	std::string value;

  public:
	Variable(const std::string var);

	virtual std::shared_ptr<BaseExpression<T>> diff(const std::string &by
	) const override;
	virtual std::shared_ptr<BaseExpression<T>>
	with_values(const std::unordered_map<std::string, T> &values
	) const override;
	virtual T resolve(void) const override;
	virtual std::string to_string(void) const override;
	virtual std::shared_ptr<BaseExpression<T>> prettify(void) const override;
};

template <typename T> class SinFunc : public BaseExpression<T> {
  private:
	std::shared_ptr<BaseExpression<T>> argument;

  public:
	explicit SinFunc(std::shared_ptr<BaseExpression<T>> argument);

	virtual std::shared_ptr<BaseExpression<T>> diff(const std::string &by
	) const override;
	virtual std::shared_ptr<BaseExpression<T>>
	with_values(const std::unordered_map<std::string, T> &values
	) const override;
	virtual T resolve(void) const override;
	virtual std::string to_string(void) const override;
	virtual std::shared_ptr<BaseExpression<T>> prettify(void) const override;
};

template <typename T> class CosFunc : public BaseExpression<T> {
  private:
	std::shared_ptr<BaseExpression<T>> argument;

  public:
	explicit CosFunc(std::shared_ptr<BaseExpression<T>> argument);

	virtual std::shared_ptr<BaseExpression<T>> diff(const std::string &by
	) const override;
	virtual std::shared_ptr<BaseExpression<T>>
	with_values(const std::unordered_map<std::string, T> &values
	) const override;
	virtual T resolve(void) const override;
	virtual std::string to_string(void) const override;
	virtual std::shared_ptr<BaseExpression<T>> prettify(void) const override;
};

template <typename T> class LnFunc : public BaseExpression<T> {
  private:
	std::shared_ptr<BaseExpression<T>> argument;

  public:
	explicit LnFunc(std::shared_ptr<BaseExpression<T>> _argument);

	virtual std::shared_ptr<BaseExpression<T>> diff(const std::string &by
	) const override;
	virtual std::shared_ptr<BaseExpression<T>>
	with_values(const std::unordered_map<std::string, T> &values
	) const override;
	virtual T resolve(void) const override;
	virtual std::string to_string(void) const override;
	virtual std::shared_ptr<BaseExpression<T>> prettify(void) const override;
};

template <typename T> class ExpFunc : public BaseExpression<T> {
  private:
	std::shared_ptr<BaseExpression<T>> argument;

  public:
	explicit ExpFunc(std::shared_ptr<BaseExpression<T>> _argument);

	virtual std::shared_ptr<BaseExpression<T>> diff(const std::string &by
	) const override;
	virtual std::shared_ptr<BaseExpression<T>>
	with_values(const std::unordered_map<std::string, T> &values
	) const override;
	virtual T resolve(void) const override;
	virtual std::string to_string(void) const override;
	virtual std::shared_ptr<BaseExpression<T>> prettify(void) const override;
};

template <typename T> class AddOp : public BaseExpression<T> {
  private:
	std::shared_ptr<BaseExpression<T>> left;
	std::shared_ptr<BaseExpression<T>> right;

  public:
	AddOp(
		const std::shared_ptr<BaseExpression<T>> &_left,
		const std::shared_ptr<BaseExpression<T>> &_right
	);

	virtual std::shared_ptr<BaseExpression<T>> diff(const std::string &by
	) const override;
	virtual std::shared_ptr<BaseExpression<T>>
	with_values(const std::unordered_map<std::string, T> &values
	) const override;
	virtual T resolve(void) const override;
	virtual std::string to_string(void) const override;
	virtual std::shared_ptr<BaseExpression<T>> prettify(void) const override;
};

template <typename T> class MultOp : public BaseExpression<T> {
  private:
	std::shared_ptr<BaseExpression<T>> left;
	std::shared_ptr<BaseExpression<T>> right;

  public:
	MultOp(
		const std::shared_ptr<BaseExpression<T>> &_left,
		const std::shared_ptr<BaseExpression<T>> &_right
	);

	virtual std::shared_ptr<BaseExpression<T>> diff(const std::string &by
	) const override;
	virtual std::shared_ptr<BaseExpression<T>>
	with_values(const std::unordered_map<std::string, T> &values
	) const override;
	virtual T resolve(void) const override;
	virtual std::string to_string(void) const override;
	virtual std::shared_ptr<BaseExpression<T>> prettify(void) const override;
};

template <typename T> class SubOp : public BaseExpression<T> {
  private:
	std::shared_ptr<BaseExpression<T>> left;
	std::shared_ptr<BaseExpression<T>> right;

  public:
	SubOp(
		const std::shared_ptr<BaseExpression<T>> &_left,
		const std::shared_ptr<BaseExpression<T>> &_right
	);

	virtual std::shared_ptr<BaseExpression<T>> diff(const std::string &by
	) const override;
	virtual std::shared_ptr<BaseExpression<T>>
	with_values(const std::unordered_map<std::string, T> &values
	) const override;
	virtual T resolve(void) const override;
	virtual std::string to_string(void) const override;
	virtual std::shared_ptr<BaseExpression<T>> prettify(void) const override;
};

template <typename T> class DivOp : public BaseExpression<T> {
  private:
	std::shared_ptr<BaseExpression<T>> left;
	std::shared_ptr<BaseExpression<T>> right;

  public:
	DivOp(
		const std::shared_ptr<BaseExpression<T>> &_left,
		const std::shared_ptr<BaseExpression<T>> &_right
	);

	virtual std::shared_ptr<BaseExpression<T>> diff(const std::string &by
	) const override;
	virtual std::shared_ptr<BaseExpression<T>>
	with_values(const std::unordered_map<std::string, T> &values
	) const override;
	virtual T resolve(void) const override;
	virtual std::string to_string(void) const override;
	virtual std::shared_ptr<BaseExpression<T>> prettify(void) const override;
};

template <typename T> class PowOp : public BaseExpression<T> {
  private:
	std::shared_ptr<BaseExpression<T>> left;
	std::shared_ptr<BaseExpression<T>> right;

  public:
	PowOp(
		const std::shared_ptr<BaseExpression<T>> &_left,
		const std::shared_ptr<BaseExpression<T>> &_right
	);

	virtual std::shared_ptr<BaseExpression<T>> diff(const std::string &by
	) const override;
	virtual std::shared_ptr<BaseExpression<T>>
	with_values(const std::unordered_map<std::string, T> &values
	) const override;
	virtual T resolve(void) const override;
	virtual std::string to_string(void) const override;
	virtual std::shared_ptr<BaseExpression<T>> prettify(void) const override;
};
} // namespace Derivative

#endif