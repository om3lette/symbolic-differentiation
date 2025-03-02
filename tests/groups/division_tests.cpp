#include "test_groups.hpp"

using namespace Derivative;
using namespace utils;

namespace test {
bool test_division(void) {
	test::Tester tester_division("Division operation");
	// clang-format off
        tester_division.register_test(
            is_equal(
                std::make_shared<DivOp<long double>>(
                    std::make_shared<Constant<long double>>(25.0L),
                    std::make_shared<Variable<long double>>("x")
                )->to_string(),
                std::string("(25) / (x)")
            ),
            "to_string 25 / x"
        );
        tester_division.register_test(
            is_equal(
                std::make_shared<DivOp<std::complex<long double>>>(
                    std::make_shared<Constant<std::complex<long double>>>(std::complex<long double>(25.0L, 10.25L)),
                    std::make_shared<Variable<std::complex<long double>>>("x")
                )->to_string(),
                std::string("((25 + 10.25i)) / (x)")
            ),
            "to_string complex (25 + 10.25i) / x"
        );
        tester_division.register_test(
            is_equal(
                std::make_shared<DivOp<long double>>(
                    std::make_shared<Constant<long double>>(25.0L),
                    std::make_shared<Variable<long double>>("x")
                )->with_values({{"x", 5.0L}})->resolve(),
                5.0L
            ),
            "with_values()->resolve() 25 / 5 = 5"
        );
        tester_division.register_test(
            is_equal(
                std::make_shared<DivOp<long double>>(
                    std::make_shared<Constant<long double>>(25.0L),
                    std::make_shared<Variable<long double>>("x")
                )->with_values({{"x", -5.0L}})->resolve(),
                -5.0L
            ),
            "with_values()->resolve() 25 / -5 = -5"
        );
        tester_division.register_test(
            is_equal(
                std::make_shared<DivOp<long double>>(
                    std::make_shared<DivOp<long double>>(
                        std::make_shared<Variable<long double>>("x"),
                        std::make_shared<Variable<long double>>("y")
                    ),
                    std::make_shared<Constant<long double>>(12.0L)
                )->with_values({{"x", 24.0L}, {"y", 2.0L}})->resolve(),
                1.0L
            ),
            "with_values(x = 24, y = 2)->resolve() x / y / 12 = 1"
        );
        tester_division.register_test(
            is_equal(
                std::make_shared<DivOp<std::complex<long double>>>(
                    std::make_shared<Constant<std::complex<long double>>>(25.0L),
                    std::make_shared<Variable<std::complex<long double>>>("x")
                )->with_values({{"x", std::complex<long double>(2.0L, 2.0L)}})->resolve(),
                std::complex<long double>(6.25L, -6.25L)
            ),
            "with_values()->resolve() complex 25 / (2 + 2i) = 6.25 - 6.25i"
        );
        tester_division.register_test(
            is_equal(
                std::make_shared<DivOp<std::complex<long double>>>(
                    std::make_shared<Constant<std::complex<long double>>>(25.0L),
                    std::make_shared<Variable<std::complex<long double>>>("x")
                )->with_values({{"x", std::complex<long double>(-2.0L, -2.0L)}})->resolve(),
                std::complex<long double>(-6.25L, 6.25L)
            ),
            "with_values()->resolve() complex 25 / (2 + 2i) = -6.25 + -6.25i"
        );
        tester_division.register_test(
            is_equal(
                std::make_shared<DivOp<long double>>(
                    std::make_shared<Variable<long double>>("x"),
                    std::make_shared<Constant<long double>>(10.0L)
                )->diff("x")->with_values({{"x", 999}})->resolve(),
                0.1L
            ),
            "diff()->resolve() d/dx x/10 = 0.1"
        );
        // Answer is 12/y. Assert that if y = 2 than answer is 6
        tester_division.register_test(
            is_equal(
                std::make_shared<DivOp<long double>>(
                    std::make_shared<Variable<long double>>("x"),
                    std::make_shared<DivOp<long double>>(
                        std::make_shared<Variable<long double>>("y"),
                        std::make_shared<Constant<long double>>(12.0L)
                    )
                )->diff("x")->with_values({{"x", 999},  {"y", 2}})->resolve(),
                6.0L
            ),
            "diff()->resolve() d/dy x / y / 12 = 12/y"
        );
	// clang-format on
	return tester_division.run_tests();
}
} // namespace test
