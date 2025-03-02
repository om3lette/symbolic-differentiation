#include "test_groups.hpp"

using namespace Derivative;
using namespace utils;

namespace test {
bool test_power(void) {
	test::Tester tester_power("Power operation");
	// clang-format off
	tester_power.register_test(
		is_equal(
			std::make_shared<PowOp<long double>>(
				std::make_shared<Constant<long double>>(25.0L),
				std::make_shared<Variable<long double>>("x")
			)->to_string(),
			std::string("(25) ^ (x)")
		),
		"to_string 25 ^ x"
	);
	tester_power.register_test(
		is_equal(
			std::make_shared<PowOp<std::complex<long double>>>(
				std::make_shared<Constant<std::complex<long double>>>(std::complex<long double>(25.0L, 10.25L)),
				std::make_shared<Variable<std::complex<long double>>>("x")
			)->to_string(),
			std::string("((25 + 10.25i)) ^ (x)")
		),
		"to_string complex (25 + 10.25i) ^ x"
	);
	tester_power.register_test(
		is_equal(
			std::make_shared<PowOp<long double>>(
				std::make_shared<Constant<long double>>(25.0L),
				std::make_shared<Variable<long double>>("x")
			)->with_values({{"x", 2.0L}})->resolve(),
			625.0L
		),
		"with_values()->resolve() 25 ^ 2 = 625"
	);
	tester_power.register_test(
		is_within_tolerance(
			std::make_shared<PowOp<long double>>(
				std::make_shared<Constant<long double>>(25.0L),
				std::make_shared<Variable<long double>>("x")
			)->with_values({{"x", -2.0L}})->resolve(),
			0.0016L
		),
		"with_values()->resolve() 25 ^ -2 = 0.0016"
	);
	tester_power.register_test(
		is_equal(
			std::make_shared<PowOp<long double>>(
				std::make_shared<PowOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Variable<long double>>("y")
				),
				std::make_shared<Constant<long double>>(2.0L)
			)->with_values({{"x", 2.0L}, {"y", 3.0L}})->resolve(),
			64.0L
		),
		"with_values(x = 2, y = 3)->resolve() x ^ y ^ 2 = 64"
	);
	tester_power.register_test(
		is_equal_complex(
			std::make_shared<PowOp<std::complex<long double>>>(
				std::make_shared<Variable<std::complex<long double>>>("x"),
				std::make_shared<Constant<std::complex<long double>>>(3.0L)
			)->with_values({{"x", std::complex<long double>(2.0L, 2.0L)}})->resolve(),
			std::complex<long double>(-16.0L, 16.0L)
		),
		"with_values()->resolve() complex (2 + 2i)^3 = -16 + 16i"
	);
	tester_power.register_test(
		is_equal_complex(
			std::make_shared<PowOp<std::complex<long double>>>(
				std::make_shared<Variable<std::complex<long double>>>("x"),
				std::make_shared<Constant<std::complex<long double>>>(3.0L)
			)->with_values({{"x", std::complex<long double>(-2.0L, -2.0L)}})->resolve(),
			std::complex<long double>(16.0L, -16.0L)
		),
		"with_values()->resolve() complex (-2 - 2i)^3 = 16 - 16i"
	);
	// Assert 3 * x^2 with x = 4 is 48
	tester_power.register_test(
		is_equal(
			std::make_shared<PowOp<long double>>(
				std::make_shared<Variable<long double>>("x"),
				std::make_shared<Constant<long double>>(3.0L)
			)->diff("x")->with_values({{"x", 4.0L}})->resolve(),
			48.0L
		),
		"diff()->resolve() d/dx x^3 = 3x^2"
	);
	// Assert 3(x + 1)^2 with x = 4 is 75
	tester_power.register_test(
		is_equal(
			std::make_shared<PowOp<long double>>(
				std::make_shared<AddOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Constant<long double>>(1.0L)
				),
				std::make_shared<Constant<long double>>(3.0L)
			)->diff("x")->with_values({{"x", 4}})->resolve(),
			75.0L
		),
		"diff()->resolve() d/dy (x + 1)^3 = 3(x + 1)^2"
	);
	// clang-format on
	return tester_power.run_tests();
}
} // namespace test