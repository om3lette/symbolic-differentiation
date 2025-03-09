#include "test_groups.hpp"

using namespace Derivative;
using namespace utils;

namespace test {
bool test_readme(void) {
	test::Tester tester_readme("README.md");
	tester_readme.register_test(
		[]() {
			Expression<long double>("my_first_var");
			Expression<long double>(42.0L);
			Expression<long double>::from_string("sin(x)/x");
			return true;
		},
		"Creating expr as const, var, from_string example"
	);
	tester_readme.register_test(
		[]() {
			Expression<long double> my_expr("x");
			my_expr = my_expr.sin().cos().ln().exp();
			return my_expr.to_string() == std::string("e^(ln(cos(sin(x))))");
		},
		"sin/cos/ln/exp example"
	);
	tester_readme.register_test(
		[]() {
			Expression<long double> my_expr_1("x");
			Expression<long double> my_expr_2(2.25L);
			auto my_expr_sum = my_expr_1 + my_expr_2; // x + 2
			auto my_expr_sub = my_expr_1 - my_expr_2; // x - 2
			auto my_expr_mul = my_expr_1 * my_expr_2; // x * 2
			auto my_expr_div = my_expr_1 / my_expr_2; // x / 2
			auto my_expr_pow = my_expr_1 ^ my_expr_2; // x ^ 2
			return my_expr_sum.prettify().to_string() ==
					   std::string("(x + 2.25)") &&
				   my_expr_sub.prettify().to_string() ==
					   std::string("(x - 2.25)") &&
				   my_expr_mul.prettify().to_string() == std::string("2.25x") &&
				   my_expr_div.prettify().to_string() ==
					   std::string("(x) / 2.25") &&
				   my_expr_pow.prettify().to_string() ==
					   std::string("(x) ^ 2.25");
		},
		"New expressions using +, -, *, /, ^"
	);
	tester_readme.register_test(
		compare_output(
			[]() {
				Expression<long double> hard_expr =
					Expression<long double>::from_string("sin(x)^2 + cos(x)^2");
				std::cout << "Calculated value: "
						  << hard_expr.resolve_with({{"x", 24.0L}}) << "\n";
			},
			"Calculated value: 1\n"
		),
		"resolve_with example"
	);
	tester_readme.register_test(
		compare_output(
			[]() {
				std::cout << "Diff out: "
						  << Expression<long double>::from_string("12x * y")
								 .diff("y")
								 .to_string()
						  << "\n";
			},
			"Diff out: (((0x + (12) * (0))) * (y) + (12x) * (1))\n"
		),
		"Diff without prettify"
	);
	tester_readme.register_test(
		compare_output(
			[]() {
				std::cout << "Diff out: "
						  << Expression<long double>::from_string("12x * y")
								 .diff("y")
								 .prettify()
								 .to_string()
						  << "\n";
			},
			"Diff out: 12x\n"
		),
		"Diff with prettify"
	);
	return tester_readme.run_tests();
}
} // namespace test