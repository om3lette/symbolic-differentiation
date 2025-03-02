#include "test_groups.hpp"

using namespace Derivative;
using namespace utils;

namespace test {
bool test_subtraction(void) {
	test::Tester tester_subtraction("Subtraction operation");
	// clang-format off
	tester_subtraction.register_test(
		is_equal(
			std::make_shared<SubOp<long double>>(
				std::make_shared<Constant<long double>>(25.0L),
				std::make_shared<Variable<long double>>("x")
			)->to_string(),
			std::string("(25 - x)")
		),
		"to_string 25 - x"
	);
	tester_subtraction.register_test(
		is_equal(
			std::make_shared<SubOp<std::complex<long double>>>(
				std::make_shared<Constant<std::complex<long double>>>(std::complex<long double>(25.0L, 10.25L)),
				std::make_shared<Variable<std::complex<long double>>>("x")
			)->to_string(),
			std::string("((25 + 10.25i) - x)")
		),
		"to_string complex (25 + 10.25i) - x"
	);
	tester_subtraction.register_test(
		is_equal(
			std::make_shared<SubOp<long double>>(
				std::make_shared<Constant<long double>>(25.0L),
				std::make_shared<Variable<long double>>("x")
			)->with_values({{"x", 42.0L}})->resolve(),
			-17.0L
		),
		"with_values()->resolve() 25 - 42 = -17"
	);
	tester_subtraction.register_test(
		is_equal(
			std::make_shared<SubOp<long double>>(
				std::make_shared<Constant<long double>>(25.0L),
				std::make_shared<Variable<long double>>("x")
			)->with_values({{"x", -42.0L}})->resolve(),
			67.0L
		),
		"with_values()->resolve() 25 - -42 = 67"
	);
	tester_subtraction.register_test(
		is_equal(
			std::make_shared<SubOp<long double>>(
				std::make_shared<SubOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Variable<long double>>("y")
				),
				std::make_shared<Constant<long double>>(12.0L)
			)->with_values({{"x", 22.0L}, {"y", 9.0L}})->resolve(),
			1.0L
		),
		"with_values(x = 22, y = 9)->resolve() x - y - 12 = 1"
	);
	tester_subtraction.register_test(
		is_equal(
			std::make_shared<SubOp<std::complex<long double>>>(
				std::make_shared<Constant<std::complex<long double>>>(25.0L),
				std::make_shared<Variable<std::complex<long double>>>("x")
			)->with_values({{"x", std::complex<long double>(2.0L, 2.0L)}})->resolve(),
			std::complex<long double>(23.0L, -2.0L)
		),
		"with_values()->resolve() complex 25 - (2 + 2i) = 23 - 2i"
	);
	tester_subtraction.register_test(
		is_equal(
			std::make_shared<SubOp<std::complex<long double>>>(
				std::make_shared<Constant<std::complex<long double>>>(25.0L),
				std::make_shared<Variable<std::complex<long double>>>("x")
			)->with_values({{"x", std::complex<long double>(-2.0L, -2.0L)}})->resolve(),
			std::complex<long double>(27.0L, 2.0L)
		),
		"with_values()->resolve() complex 25 - (-2 - 2i) = 27 + 2i"
	);
	tester_subtraction.register_test(
		is_equal(
			std::make_shared<SubOp<long double>>(
				std::make_shared<SubOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Variable<long double>>("y")
				),
				std::make_shared<Constant<long double>>(12.0L)
			)->diff("x")->resolve(),
			std::make_shared<Variable<long double>>("x")->diff("x")->resolve()
		),
		"diff()->resolve() d/dx x - y - 12 = d/dx x"
	);
	tester_subtraction.register_test(
		is_equal(
			std::make_shared<SubOp<long double>>(
				std::make_shared<SubOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Variable<long double>>("y")
				),
				std::make_shared<Constant<long double>>(12.0L)
			)->diff("x")->resolve(),
			1.0L
		),
		"diff()->resolve() d/dx x - y - 12 = 1"
	);
	// clang-format on
	return tester_subtraction.run_tests();
}
} // namespace test
