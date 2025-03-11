# Symbolic differentiation library

Library written as a homework assignment during HSE: SE C++ course.

> [!WARNING]
> This is my first experience with C++

## Functionality

Library provides a class `Expression<T>` that can be used to evaluate and differentiate math expressions.\
The supported types are `long double` and `std::complex<long double>`

## Initialization

There are multiple ways to create an `Expression<T>`

- Create a variable: `Expression<long double>("my_first_var");`
- Create a constant `Expression<long double>(42.0L);`
- Parse a given string `Expression<long double>::from_string("sin(x)/x");`

If you already have an expression you can use it's methods to alter it further

```cpp
Expression<long double> my_expr("x");
my_expr = my_expr.sin().cos().ln().exp();
std::cout << my_expr.to_string() << "\n";
// Output: e^(ln(cos(sin(x))))
```

Or combine multiple expressions using `+`, `-`, `*`, `/`, `^`

```cpp
Expression<long double> my_expr_1("x");
Expression<long double> my_expr_2(2.25L);
auto my_expr_sum = my_expr_1 + my_expr_2; // x + 2.25
auto my_expr_sub = my_expr_1 - my_expr_2; // x - 2.25
auto my_expr_mul = my_expr_1 * my_expr_2; // x * 2.25
auto my_expr_div = my_expr_1 / my_expr_2; // x / 2.25
auto my_expr_pow = my_expr_1 ^ my_expr_2; // x ^ 2.25

```

## Calculations

There are two main methods: `resolve_with` and `diff`

### `resolve_with`

Allows to substitute variables with values (of the template type specified on creation)

```cpp
Expression<long double> hard_expr = Expression<long double>::from_string("sin(x)^2 + cos(x)^2");
// To calculate it's value use resolve_with
std::cout << "Calculated value: " << hard_expr.resolve_with({{"x", 24.0L}}) << "\n";
// Calculated value: 1
```

If a variable which is not present in expression is passed to `resolve_with` nothing will happen.\
But if one or more are missing you will get a `std::runtime_error("Variable cannot be resolved")`

`resolve_with` is an alias for `my_expr.with_values(...).resolve()`. So if you only want to substitute some variables you can call `with_values` instead.

### `diff`

Calculates a derivative of the given expression (note that it returns a new `Expression` not a `std::string`)

```cpp
std::cout << "Diff out: " << Expression<long double>::from_string("12x * y").diff("y").to_string() << "\n";
// Diff out: (((0x + (12) * (0))) * (y) + (12x) * (1))
```

It is not exactly what we have come to expect, is it?
To partlially fix the problem we can apply `prettify`. It applies basic simplification such as

- x + 0 = 0 + x = x - 0 = x
- 0 - x = -x
- x \* 0 = 0 \* x = 0
- x / 1 = x
- x^1 = x, 0^x = 0
- const + const -> const (evaluates the expression for any operator)

```cpp
std::cout << "Diff out: " << Expression<long double>::from_string("12x * y").diff("y").prettify().to_string() << "\n";
// Diff out: 12x
```

## Makefile

### Targets

- `all` - builds the entire project (`test.build && console.build`)
- `test` - runs test executeable
- `test.build` - builds test executable
- `console.build` - builds executable for console use
- `coverage` - `test.build` is a prerequisite. Runs tests and generates coverage report
- `clean` - deletes build/coverage folders

### Arguments:

- `RELEASE` values: `1` - adds optimization flags when compiling

## Docker image

```bash
# Build image
docker build -t symbolic-diff .
```

```bash
# Run tests
docker run symbolic-diff
```

```bash
# Run eval/diff
docker run symbolic-diff ./differentiator --eval "x + y" x=2 y=3 && \
docker run symbolic-diff ./differentiator --diff "sin(x)/x" --by x
```
