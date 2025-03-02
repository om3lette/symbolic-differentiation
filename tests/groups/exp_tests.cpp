#include "test_groups.hpp"

using namespace Derivative;
using namespace utils;

namespace test {
bool test_exp(void) {
	test::Tester tester_exp("Exp function");
	// clang-format off
	tester_exp.register_test(
		is_equal(
			std::make_shared<ExpFunc<long double>>(
				std::make_shared<Variable<long double>>("x")
			)->to_string(),
			std::string("e^(x)")
		),
		"to_string e^x"
	);
	tester_exp.register_test(
		is_equal(
			std::make_shared<ExpFunc<long double>>(
				std::make_shared<MultOp<long double>>(
					std::make_shared<Constant<long double>>(4.0L),
					std::make_shared<Variable<long double>>("x")
				)
			)->to_string(),
			std::string("e^((4) * (x))")
		),
		"to_string e^(4 * x)"
	);
	tester_exp.register_test(
		is_within_tolerance(
			std::make_shared<ExpFunc<long double>>(
				std::make_shared<MultOp<long double>>(
					std::make_shared<Constant<long double>>(4.0L),
					std::make_shared<Variable<long double>>("x")
				)
			)->with_values({{"x", 2}})->resolve(),
			std::pow(e, 8)
		),
		"with_values(x = 2) e^(4x) = e^8"
	);
	tester_exp.register_test(
		is_within_tolerance(
			std::make_shared<ExpFunc<long double>>(
				std::make_shared<DivOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Constant<long double>>(3)
				)
			)->with_values({{"x", -1}})->resolve(),
			std::pow(e, -1.0L / 3)
		),
		"with_values(x = -1) e^(x/3) = 1/e^3"
	);
	long double e_by_six = e / 6.0L;
	tester_exp.register_test(
		is_within_tolerance(
			std::make_shared<ExpFunc<long double>>(
				std::make_shared<DivOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Constant<long double>>(6)
				)
			)->diff("x")->with_values({{"x", 6}})->resolve(),
			e_by_six
		),
		"d/dx e^(x/6) = 1/6 * e^(x/6)"
	);
	// clang-format on
	return tester_exp.run_tests();
}
} // namespace test
