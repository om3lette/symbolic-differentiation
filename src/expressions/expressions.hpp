#ifndef EXPRESSIONS_HPP
#define EXPRESSIONS_HPP

#include <complex>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace Derivative {

template <typename T> class BaseExpression {
  public:
	virtual ~BaseExpression() = default;

	virtual std::shared_ptr<BaseExpression<T>> diff(const std::string &by
	) const = 0;
	virtual std::shared_ptr<BaseExpression<T>>
	with_values(const std::unordered_map<std::string, T> &values) const = 0;
	virtual T resolve(void) const = 0;
	virtual std::string to_string(void) const = 0;
};

template <typename T> class Expression {
  public:
	Expression(T number);
	Expression(std::string &variable);

	Expression<T> operator+(const Expression &other);
	Expression<T> &operator+=(const Expression &other);

	Expression<T> operator-(const Expression &other);
	Expression<T> &operator-=(const Expression &other);

	Expression<T> operator*(const Expression &other);
	Expression<T> &operator*=(const Expression &other);

	Expression<T> operator/(const Expression &other);
	Expression<T> &operator/=(const Expression &other);

	virtual std::shared_ptr<BaseExpression<T>> diff(const std::string &by
	) const override;
	virtual std::shared_ptr<BaseExpression<T>>
	with_values(const std::unordered_map<std::string, T> &values
	) const override;
	virtual T resolve(void) const override;
	virtual std::string to_string(void) const override;

  private:
	Expression(std::shared_ptr<BaseExpression<T>> expression_impl);
	std::shared_ptr<BaseExpression<T>> value;
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
};

template <typename T> class Variable : public BaseExpression<T> {
  private:
	std::string value;

  public:
	explicit Variable(std::string var);

	virtual std::shared_ptr<BaseExpression<T>> diff(const std::string &by
	) const override;
	virtual std::shared_ptr<BaseExpression<T>>
	with_values(const std::unordered_map<std::string, T> &values
	) const override;
	virtual T resolve(void) const override;
	virtual std::string to_string(void) const override;
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
};
} // namespace Derivative

#endif