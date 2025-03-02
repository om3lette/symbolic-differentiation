#include "test_groups.hpp"

using namespace Derivative;
using namespace utils;

namespace test {
bool test_sin(void) {
	test::Tester tester_sin("Sin function");
	// clang-format off
	tester_sin.register_test(
		is_equal(
			std::make_shared<SinFunc<long double>>(
				std::make_shared<Variable<long double>>("x")
			)->to_string(),
			std::string("sin(x)")
		),
		"to_string sin(x)"
	);
	tester_sin.register_test(
		is_equal(
			std::make_shared<SinFunc<long double>>(
				std::make_shared<MultOp<long double>>(
					std::make_shared<Constant<long double>>(4.0L),
					std::make_shared<Variable<long double>>("x")
				)
			)->to_string(),
			std::string("sin((4) * (x))")
		),
		"to_string sin(4 * x)"
	);
	tester_sin.register_test(
		is_within_tolerance(
			std::make_shared<SinFunc<long double>>(
				std::make_shared<MultOp<long double>>(
					std::make_shared<Constant<long double>>(4.0L),
					std::make_shared<Variable<long double>>("x")
				)
			)->with_values({{"x", pi}})->resolve(),
			0.0L
		),
		"with_values(x = pi) sin(4x) = 0"
	);
	tester_sin.register_test(
		is_within_tolerance(
			std::make_shared<SinFunc<long double>>(
				std::make_shared<DivOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Constant<long double>>(6)
				)
			)->with_values({{"x", pi}})->resolve(),
			0.5L
		),
		"with_values(x = pi) sin(x/6) = 0.5"
	);
	tester_sin.register_test(
		is_within_tolerance(
			std::make_shared<SinFunc<long double>>(
				std::make_shared<DivOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Constant<long double>>(3)
				)
			)->diff("x")->with_values({{"x", pi}})->resolve(),
			1.0L / 6
		),
		"d/dx sin(x/3) = cos(x/3)/3"
	);
	// clang-format on
	return tester_sin.run_tests();
}
} // namespace test
