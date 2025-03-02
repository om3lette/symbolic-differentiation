#include "test_groups.hpp"

using namespace Derivative;
using namespace utils;

namespace test {
bool test_ln(void) {
	test::Tester tester_ln("Ln function");
	// clang-format off
	tester_ln.register_test(
		is_equal(
			std::make_shared<LnFunc<long double>>(
				std::make_shared<Variable<long double>>("x")
			)->to_string(),
			std::string("ln(x)")
		),
		"to_string ln(x)"
	);
	tester_ln.register_test(
		is_equal(
			std::make_shared<LnFunc<long double>>(
				std::make_shared<AddOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Constant<long double>>(1.0L)
				)
			)->to_string(),
			std::string("ln((x + 1))")
		),
		"to_string ln(x + 1)"
	);
	tester_ln.register_test(
		is_equal(
			std::make_shared<LnFunc<long double>>(
					std::make_shared<Variable<long double>>("x")
			)->with_values({{"x", e}})->resolve(),
			1.0L
		),
		"ln(e) = 1"
	);
	tester_ln.register_test(
		is_within_tolerance(
			std::make_shared<LnFunc<long double>>(
					std::make_shared<Variable<long double>>("x")
			)->with_values({{"x", std::pow(e, 15)}})->resolve(),
			15.0L
		),
		"ln(e) = 1"
	);
	tester_ln.register_test(
		is_equal(
			std::make_shared<LnFunc<long double>>(
				std::make_shared<MultOp<long double>>(
					std::make_shared<Constant<long double>>(10000.0L),
					std::make_shared<Variable<long double>>("x")
				)
			)->diff("x")->with_values({{"x", 1}})->resolve(),
			1.0L
		),
		"ln'(10000 * x) = 1/x"
	);
	tester_ln.register_test(
		is_equal(
			std::make_shared<LnFunc<long double>>(
				std::make_shared<AddOp<long double>>(
					std::make_shared<Variable<long double>>("x"),
					std::make_shared<Constant<long double>>(10.0L)
				)
			)->diff("x")->with_values({{"x", 10}})->resolve(),
			0.05L
		),
		"ln'(x + 10) = 1/(x + 10)"
	);
	tester_ln.register_exception_test(
		[](){
			std::make_shared<LnFunc<long double>>(
				std::make_shared<Constant<long double>>(-10.0L)
			)->resolve();
		},
		"ln with negative argument (error)"
	);
	tester_ln.register_exception_test(
		[](){
			std::make_shared<LnFunc<std::complex<long double>>>(
				std::make_shared<Constant<std::complex<long double>>>(
					std::complex<long double>(1.0L, 1.0L)
				)
			)->resolve();
		},
		"ln with complex argument (error)"
	);
	// clang-format on
	return tester_ln.run_tests();
}
} // namespace test
