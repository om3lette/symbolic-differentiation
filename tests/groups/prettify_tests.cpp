#include "test_groups.hpp"

using namespace Derivative;
using namespace utils;

namespace test {
bool test_prettify(void) {
	test::Tester tester_prettify("");
	tester_prettify.register_test(
		is_within_tolerance(
			std::stold(Expression<long double>::from_string("e^(1 + 1)x")
						   .diff("x")
						   .prettify()
						   .to_string()),
			std::pow(e, 2), 1.0e-4L
		),
		"d/dx e^(1 + 1)x"
	);
	// From multiplication tests to_string
	tester_prettify.register_test(
		is_equal(
			Expression<long double>::from_string("12x * y")
				.diff("y")
				.prettify()
				.to_string(),
			std::string("12x")
		),
		"d/dy x * y * 12 = 12x"
	);
	// From parser tests
	tester_prettify.register_test(
		is_equal(
			Expression<long double>::from_string("sin(x) / (x + 10)")
				.diff("x")
				.prettify()
				.to_string(),
			std::string(
				"(((cos(x)) * ((x + 10)) - sin(x))) / (((x + 10)) ^ (2))"
			)
		),
		"d/dx sin(x) / (x + 10)"
	);
	// From expression tests
	tester_prettify.register_test(
		is_equal(
			Expression<long double>::from_string("sin(x)/x")
				.diff("x")
				.prettify()
				.to_string(),
			std::string("(((cos(x)) * (x) - sin(x))) / ((x) ^ (2))")
		),
		"d/dx sin(x)/x = (cos(x) * x - sin(x)) / x^2"
	);
	return tester_prettify.run_tests();
}
} // namespace test