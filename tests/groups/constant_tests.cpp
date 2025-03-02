#include "test_groups.hpp"

using namespace Derivative;
using namespace utils;

namespace test {
bool test_constant(void) {
	test::Tester tester_constant("Constant expression");
	tester_constant.register_test(
		is_equal(Constant<long double>(2.0).to_string(), std::string("2")),
		"to_string (long double)"
	);
	tester_constant.register_test(
		is_equal(Constant<long double>(2.25).to_string(), std::string("2.25")),
		"to_string (long double with fraction)"
	);
	tester_constant.register_test(
		is_equal(
			Constant<std::complex<long double>>(std::complex<long double>(1, 1))
				.to_string(),
			std::string("(1 + i)")
		),
		"to_string (complex - real and imag coef = 1)"
	);
	tester_constant.register_test(
		is_equal(
			Constant<std::complex<long double>>(std::complex<long double>(0, 1))
				.to_string(),
			std::string("i")
		),
		"to_string (complex - no real part)"
	);
	tester_constant.register_test(
		is_equal(
			Constant<std::complex<long double>>(std::complex<long double>(1, 0))
				.to_string(),
			std::string("1")
		),
		"to_string (complex - no imag part)"
	);
	tester_constant.register_test(
		is_equal(
			Constant<std::complex<long double>>(std::complex<long double>(0, 0))
				.to_string(),
			std::string("0")
		),
		"to_string (complex = 0)"
	);
	tester_constant.register_test(
		is_equal(
			Constant<std::complex<long double>>(
				std::complex<long double>(-2, -5)
			)
				.to_string(),
			std::string("(-2 - 5i)")
		),
		"to_string (complex - negative imag part)"
	);
	tester_constant.register_test(
		is_equal(Constant<long double>(10.25L).diff("x")->resolve(), 0.0L),
		"diff positive value - 0"
	);
	tester_constant.register_test(
		is_equal(Constant<long double>(-9000.2025L).diff("z")->resolve(), 0.0L),
		"diff negative value - 0"
	);
	tester_constant.register_test(
		is_equal(
			Constant<long double>(100.0L).with_values({{"x", 20}})->resolve(),
			100.0L
		),
		"with_values returns the same expression"
	);
	tester_constant.register_test(
		is_equal(Constant<long double>(100.0L).resolve(), 100.0L),
		"resolve returns the same expression"
	);
	return tester_constant.run_tests();
}
} // namespace test