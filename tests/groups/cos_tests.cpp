#include "test_groups.hpp"

using namespace Derivative;
using namespace utils;

namespace test {
bool test_cos(void) {
	test::Tester tester_cos("Cos function");
	// clang-format off
		tester_cos.register_test(
			is_equal(
				std::make_shared<CosFunc<long double>>(
					std::make_shared<Variable<long double>>("x")
				)->to_string(),
				std::string("cos(x)")
			),
			"to_string cos(x)"
		);
		tester_cos.register_test(
			is_equal(
				std::make_shared<CosFunc<long double>>(
					std::make_shared<MultOp<long double>>(
						std::make_shared<Constant<long double>>(4.0L),
						std::make_shared<Variable<long double>>("x")
					)
				)->to_string(),
				std::string("cos(4x)")
			),
			"to_string cos(4 * x)"
		);
		tester_cos.register_test(
			is_within_tolerance(
				std::make_shared<CosFunc<long double>>(
					std::make_shared<MultOp<long double>>(
						std::make_shared<Constant<long double>>(4.0L),
						std::make_shared<Variable<long double>>("x")
					)
				)->with_values({{"x", pi/8}})->resolve(),
				0.0L
			),
			"with_values(x = pi/8) cos(4x) = 0"
		);
		tester_cos.register_test(
			is_within_tolerance(
				std::make_shared<CosFunc<long double>>(
					std::make_shared<DivOp<long double>>(
						std::make_shared<Variable<long double>>("x"),
						std::make_shared<Constant<long double>>(3)
					)
				)->with_values({{"x", pi}})->resolve(),
				0.5L
			),
			"with_values(x = pi) cos(x/3) = 0.5"
		);
		tester_cos.register_test(
			is_within_tolerance(
				std::make_shared<CosFunc<long double>>(
					std::make_shared<DivOp<long double>>(
						std::make_shared<Variable<long double>>("x"),
						std::make_shared<Constant<long double>>(6)
					)
				)->diff("x")->with_values({{"x", pi}})->resolve(),
				-1.0L / 12
			),
			"d/dx cos(x/6) = -1 * sin(x/6)/6"
		);
	// clang-format on
	return tester_cos.run_tests();
}
} // namespace test
