#include "test_groups.hpp"

using namespace Derivative;
using namespace utils;

namespace test {
bool test_expression(void) {
	test::Tester tester_expression("Expression function");
	tester_expression.register_test(
		is_within_tolerance(
			(Expression<long double>(5.0L) + Expression<long double>("x"))
				.exp()
				.resolve_with({{"x", 2.0L}}),
			std::pow(e, 7)
		),
		"e^(5 + x) from Expressions"
	);
	tester_expression.register_test(
		is_equal(
			((Expression<long double>("x") + Expression<long double>(42.0L)) *
			 (Expression<long double>(2.0L) +
			  Expression<long double>("long_var")))
				.to_string(),
			std::string("((x + 42)) * ((2 + long_var))")
		),
		"(x + 42) * (2 + long_var) from Expressions"
	);
	tester_expression.register_test(
		[]() {
			Expression<long double> expression = Expression<long double>(2.0L);
			expression *= Expression<long double>(4.0L);
			expression += Expression<long double>("super_long_var");
			expression -= Expression<long double>("var");
			expression ^= Expression<long double>(2);
			return expression.resolve_with(
					   {{"super_long_var", 10.0L}, {"var", 2.0L}}
				   ) == 256.0L;
		},
		"with(s = 10, v = 2): (4 + s - v)^2 (+= *= -= ^=)"
	);
	tester_expression.register_test(
		is_within_tolerance(Expression<long double>(pi).sin().resolve(), 0.0L),
		"sin(pi) = 0"
	);
	tester_expression.register_test(
		is_within_tolerance(
			Expression<long double>(pi / 2).cos().resolve(), 0.0L
		),
		"cos(pi/2) = 0"
	);
	tester_expression.register_test(
		is_within_tolerance(
			Expression<long double>("x").ln().resolve_with({{"x", e}}), 1.0L
		),
		"with(x = e) ln(x) = 1"
	);
	tester_expression.register_test(
		is_equal(
			(Expression<long double>("x").sin() / Expression<long double>("x"))
				.diff("x")
				.to_string(),
			std::string(
				"((((cos(x)) * (1)) * (x) - (sin(x)) * (1))) / ((x) ^ 2)"
			)
		),
		"d/dx sin(x)/x = (cos(x) * x - sin(x)) / x^2"
	);
	tester_expression.register_test(
		is_equal(
			Expression<long double>(Expression<long double>("x").sin())
				.to_string(),
			Expression<long double>("x").sin().to_string()
		),
		"Expression from Expression"
	);
	tester_expression.register_test(
		is_equal(
			(Expression<long double>("x") /= 10).resolve_with({{"x", 10}}), 1.0L
		),
		"with(x = 10) x /= 10 => 1"
	);
	tester_expression.register_test(
		[]() {
			Expression<long double> x = Expression<long double>("x");
			x -= 25;
			return x.with_values({{"x", 999}}).resolve() == 974.0L;
		},
		"Expression with_values"
	);
	tester_expression.register_test(
		[]() {
			Expression<long double> x("x");
			Expression<long double> y("y");
			x = y;
			return x.resolve_with({{"y", 999}}) == 999.0L;
		},
		"Expression copy operator"
	);
	tester_expression.register_test(
		[]() {
			Expression<long double> y("y");
			Expression<long double> x(y);
			return x.resolve_with({{"y", 999}}) == 999.0L;
		},
		"Expression copy constructor"
	);
	tester_expression.register_test(
		is_equal(
			Expression<long double>("UpPeRcAsE_nAmE")
				.resolve_with({{"uppercase_name", 42.0L}}),
			42.0L
		),
		"Uppercase variable is treated as lower case"
	);

	return tester_expression.run_tests();
}
} // namespace test
