#include "test_groups.hpp"

using namespace Derivative;
using namespace utils;

namespace test {
bool test_variable(void) {
	test::Tester tester_variable("Variable");
	tester_variable.register_test(
		is_equal(Variable<long double>("x").diff("x")->resolve(), 1.0L),
		"d/dx x = 1"
	);
	tester_variable.register_test(
		is_equal(Variable<long double>("x").diff("y")->resolve(), 0.0L),
		"d/dy x = 0"
	);
	tester_variable.register_test(
		is_equal(
			Variable<long double>("x").with_values({{"x", 42.0L}})->resolve(),
			42.0L
		),
		"x with_values(x = 42)"
	);
	tester_variable.register_test(
		is_equal(
			Variable<std::complex<long double>>("z")
				.with_values({{"z", std::complex<long double>(2, 2)}})
				->resolve(),
			std::complex<long double>(2, 2)
		),
		"z with_values(z = 2 + 2i)"
	);
	tester_variable.register_test(
		is_equal(
			Variable<std::complex<long double>>("z")
				.with_values({{"z", std::complex<long double>(2, 2)}})
				->to_string(),
			std::string("(2 + 2i)")
		),
		"z with_values(z = 2 + 2i)->to_string()"
	);
	tester_variable.register_test(
		is_equal(
			Variable<long double>("long_var").to_string(),
			std::string("long_var")
		),
		"to_string long_var"
	);
	tester_variable.register_exception_test(
		[]() {
			Variable<long double>("x").with_values({{"y", 2.0L}})->resolve();
		},
		"x with_values(y) into resolve (runtime error)"
	);

	return tester_variable.run_tests();
}
} // namespace test
