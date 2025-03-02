#include "test_groups.hpp"

using namespace Derivative;
using namespace utils;

namespace test {
bool test_multiplication(void) {
	test::Tester tester_multiplication("Multiplication operation");
	// clang-format off
        tester_multiplication.register_test(
            is_equal(
                std::make_shared<MultOp<long double>>(
                    std::make_shared<Constant<long double>>(25.0L),
                    std::make_shared<Variable<long double>>("x")
                )->to_string(),
                std::string("(25) * (x)")
            ),
            "to_string 25 * x"
        );
        tester_multiplication.register_test(
            is_equal(
                std::make_shared<MultOp<std::complex<long double>>>(
                    std::make_shared<Constant<std::complex<long double>>>(std::complex<long double>(25.0L, 10.25L)),
                    std::make_shared<Variable<std::complex<long double>>>("x")
                )->to_string(),
                std::string("((25 + 10.25i)) * (x)")
            ),
            "to_string complex (25 + 10.25i) * x"
        );
        tester_multiplication.register_test(
            is_equal(
                std::make_shared<MultOp<long double>>(
                    std::make_shared<Constant<long double>>(25.0L),
                    std::make_shared<Variable<long double>>("x")
                )->with_values({{"x", 42.0L}})->resolve(),
                1050.0L
            ),
            "with_values()->resolve() 25 * 42 = 1050"
        );
        tester_multiplication.register_test(
            is_equal(
                std::make_shared<MultOp<long double>>(
                    std::make_shared<Constant<long double>>(25.0L),
                    std::make_shared<Variable<long double>>("x")
                )->with_values({{"x", -42.0L}})->resolve(),
                -1050.0L
            ),
            "with_values()->resolve() 25 * -42 = -1050"
        );
        tester_multiplication.register_test(
            is_equal(
                std::make_shared<MultOp<long double>>(
                    std::make_shared<MultOp<long double>>(
                        std::make_shared<Variable<long double>>("x"),
                        std::make_shared<Variable<long double>>("y")
                    ),
                    std::make_shared<Constant<long double>>(12.0L)
                )->with_values({{"x", 22.0L}, {"y", 9.0L}})->resolve(),
                2376.0L
            ),
            "with_values(x = 22, y = 9)->resolve() x * y * 12 = 2376"
        );
        tester_multiplication.register_test(
            is_equal(
                std::make_shared<MultOp<std::complex<long double>>>(
                    std::make_shared<Constant<std::complex<long double>>>(25.0L),
                    std::make_shared<Variable<std::complex<long double>>>("x")
                )->with_values({{"x", std::complex<long double>(2.0L, 2.0L)}})->resolve(),
                std::complex<long double>(50.0L, 50.0L)
            ),
            "with_values()->resolve() complex 25 * (2 + 2i) = 50 + 50i"
        );
        tester_multiplication.register_test(
            is_equal(
                std::make_shared<MultOp<std::complex<long double>>>(
                    std::make_shared<Constant<std::complex<long double>>>(25.0L),
                    std::make_shared<Variable<std::complex<long double>>>("x")
                )->with_values({{"x", std::complex<long double>(-2.0L, -2.0L)}})->resolve(),
                std::complex<long double>(-50.0L, -50.0L)
            ),
            "with_values()->resolve() complex 25 * (-2 - 2i) = -50 - 50i"
        );
        tester_multiplication.register_test(
            is_equal(
                std::make_shared<MultOp<long double>>(
                    std::make_shared<Variable<long double>>("x"),
                    std::make_shared<Constant<long double>>(12.0L)
                )->diff("x")->to_string(),
                std::string("((1) * (12) + (x) * (0))")
            ),
            "diff()->resolve() d/dx 12x = 12"
        );
        tester_multiplication.register_test(
            is_equal(
                std::make_shared<MultOp<long double>>(
                    std::make_shared<MultOp<long double>>(
                        std::make_shared<Variable<long double>>("x"),
                        std::make_shared<Variable<long double>>("y")
                    ),
                    std::make_shared<Constant<long double>>(12.0L)
                )->diff("y")->to_string(),
                std::string("((((0) * (y) + (x) * (1))) * (12) + ((x) * (y)) * (0))")
            ),
            "diff()->resolve() d/dy x * y * 12 = 12x"
        );
	// clang-format on
	return tester_multiplication.run_tests();
}
} // namespace test
