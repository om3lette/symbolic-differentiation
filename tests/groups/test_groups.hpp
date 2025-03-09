#ifndef TEST_GROUPS
#define TEST_GROUPS

#include "../../src/expressions/expressions.hpp"
#include "../../src/parser/Parser.hpp"
#include "../Tester.hpp"
#include "../utils.hpp"
#include "test_groups.hpp"

namespace test {
bool test_parser(void);
bool test_lexer(void);

bool test_constant(void);
bool test_variable(void);

bool test_sin(void);
bool test_cos(void);
bool test_ln(void);
bool test_exp(void);

bool test_addition(void);
bool test_subtraction(void);
bool test_multiplication(void);
bool test_division(void);
bool test_power(void);

bool test_prettify(void);

bool test_expression(void);
bool test_readme(void);
} // namespace test

#endif