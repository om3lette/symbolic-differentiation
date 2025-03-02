#include "../../src/expressions/expressions.hpp"
#include "../../src/parser/Parser.hpp"
#include "../Tester.hpp"
#include "../utils.hpp"
#include "test_groups.hpp"

using namespace Derivative;
using namespace utils;

//==========================================================================
// Lexer l = Lexer("e2");
// Token m;
// while (m.type != TokenType::End) {
// 	m = l.next_token();
// 	std::cout << m.value << ", ";
// };
// std::cout << Parser<long double>("e2").parse().to_string() << std::endl;
namespace test {
bool test_parser(void) {
	test::Tester tester_parser("Parser");
	tester_parser.register_test(
		is_equal(
			Parser<long double>("(2 + 4x) / 3")
				.parse()
				.resolve_with({{"x", 10.0L}}),
			14.0L
		),
		"with(x = 10) (2 + 4x) / 3 = 14"
	);
	tester_parser.register_test(
		is_equal(
			Parser<long double>("10(2 + 4x)")
				.parse()
				.resolve_with({{"x", 10.0L}}),
			420.0L
		),
		"with(x = 10) 10(2 + 4x) = 420"
	);
	tester_parser.register_test(
		is_equal(
			Parser<long double>("(2 + 4x) / 3")
				.parse()
				.resolve_with({{"x", 10.0L}}),
			14.0L
		),
		"with(x = 10) (2 + 4x) / 3 = 14"
	);
	tester_parser.register_test(
		is_within_tolerance(
			Parser<long double>("e^(2 + 4x) / e")
				.parse()
				.resolve_with({{"x", 1.0L}}),
			std::pow(e, 5)
		),
		"with(x = 1) e^(2 + 4x) / e = e^5"
	);
	tester_parser.register_test(
		is_within_tolerance(
			Parser<long double>("e^2").parse().resolve(), std::pow(e, 2)
		),
		"e^2 = 7.389"
	);
	tester_parser.register_test(
		is_within_tolerance(Parser<long double>("2e").parse().resolve(), e * 2),
		"2e = 5.436"
	);
	tester_parser.register_test(
		is_within_tolerance(
			Parser<long double>("2e").parse().resolve(),
			Parser<long double>("e * 2").parse().resolve()
		),
		"2e = e * 2"
	);
	tester_parser.register_test(
		is_within_tolerance(
			Parser<long double>("2 * e").parse().resolve(),
			Parser<long double>("e * 2").parse().resolve()
		),
		"e * 2 = 2 * e"
	);
	tester_parser.register_test(
		is_within_tolerance(
			Parser<long double>("2e").parse().resolve(),
			Parser<long double>("2 * e").parse().resolve()
		),
		"2e ≈ 5.436"
	);
	tester_parser.register_test(
		is_within_tolerance(
			Parser<long double>("exp(2 + 4x) / e")
				.parse()
				.resolve_with({{"x", 1.0L}}),
			Parser<long double>("e^(2 + 4x) / e")
				.parse()
				.resolve_with({{"x", 1.0L}})
		),
		"with(x = 1) exp(2 + 4x) / e = e^(2 + 4x) / e = e^5"
	);
	tester_parser.register_test(
		is_equal(
			Parser<long double>("(10 + 5) * 2 - 4").parse().resolve(), 26.0L
		),
		"(10 + 5) * 2 - 4 = 26.0L"
	);
	tester_parser.register_test(
		is_equal(
			Parser<long double>("(10 + 5) * 2 - 4").parse().resolve(), 26.0L
		),
		"(10 + 5) * 2 - 4 = 2 * (10 + 5) - 4"
	);
	tester_parser.register_test(
		is_equal(
			Parser<long double>("((((10 + 5) * (2 - 4))))").parse().resolve(),
			-30.0L
		),
		"((((10 + 5) * (2 - 4)))) = -30"
	);
	tester_parser.register_test(
		is_within_tolerance(
			Parser<long double>("sin(cos(ln(2e))) * x")
				.parse()
				.resolve_with({{"x", 2.0L}}),
			-0.2434861307231135L
		),
		"with(x = 2) sin(cos(ln(2e))) * x"
	);
	tester_parser.register_test(
		is_within_tolerance(
			Parser<long double>("sin(x) / (x + 10)")
				.parse()
				.diff("x")
				.resolve_with({{"x", pi}}),
			-1.0L / (10.0L + pi)
		),
		"d/dx sin(x) / (x + 10) with(x = pi) = -1/(10 + pi)"
	);
	tester_parser.register_test(
		is_equal_complex(
			Parser<std::complex<long double>>("4 + 2i").parse().resolve(),
			std::complex<long double>(4, 2)
		),
		"4 + 2i"
	);
	tester_parser.register_test(
		is_equal_complex(
			Parser<std::complex<long double>>("-4 - 2i").parse().resolve(),
			std::complex<long double>(-4, -2)
		),
		"-4 - 2i"
	);
	tester_parser.register_test(
		is_equal_complex(
			Parser<std::complex<long double>>("-5(4 + 2i) + x")
				.parse()
				.resolve_with({{"x", std::complex<long double>(1.0L, 11.0L)}}),
			std::complex<long double>(-19.0L, 1.0L)
		),
		"with(x = 1 + 11i) -5(4 + 2i) + x = -19 + i"
	);
	tester_parser.register_test(
		is_equal_complex(
			Parser<std::complex<long double>>("-5x(4 + 2i)")
				.parse()
				.resolve_with({{"x", std::complex<long double>(2.0L, 2.0L)}}),
			std::complex<long double>(-20.0L, -60.0L)
		),
		"with(x = 2 + 2i) -5x(4 + 2i) = -20 - 60i"
	);
	tester_parser.register_test(
		is_equal_complex(
			Parser<std::complex<long double>>("-5x(4 + 2i) / 4y(-2 + i)")
				.parse()
				.resolve_with(
					{{"x", std::complex<long double>(5.0L, -4.0L)},
					 {"y", std::complex<long double>(10.0L, 17.0L)}}
				),
			std::complex<long double>(1475.0L, 562.5L)
		),
		"with(x = 2 + 2i, y = 10 + 17i) -5x(4 + 2i) / 4y(-2 + i) = 1475 + "
		"562.5i"
	);
	return tester_parser.run_tests();
}
} // namespace test