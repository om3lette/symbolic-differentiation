#include "./expressions/expressions.hpp"
#include "string_utils.hpp"

#include <iostream>
#include <regex>
#include <stdexcept>
#include <unordered_map>

using VariableType = std::unordered_map<std::string, long double>;
using ComplexVariableType =
	std::unordered_map<std::string, std::complex<long double>>;

bool is_complex(const std::string &val) {
	return val.find("i") != std::string::npos;
}

template <typename T, typename VarMap>
std::string run_task(
	Derivative::Expression<T> expr, bool to_diff, bool to_eval,
	const std::string &diff_by, const VarMap &values
) {
	std::stringstream oss;
	if (to_diff) oss << "Differentiated: " << expr.diff(diff_by).to_string();
	if (to_eval) oss << "Evaluated: " << expr.resolve_with(values);
	return oss.str();
}

int main(int argc, char *argv[]) {
	std::string expression_string, diff_by;
	bool eval_expr = false, diff_expr = false, contains_complex = false;
	VariableType variables;
	ComplexVariableType complex_variables;
	std::regex complex_regex(R"(([+-]?\d*\.?\d+)([+-]\d*\.?\d*)i)");

	for (int i = 1; i < argc; i++) {
		std::string arg = argv[i];
		if (arg == "--eval" || arg == "--diff") {
			if (++i >= argc)
				throw std::invalid_argument("No value specified for " + arg);
			expression_string = argv[i];
			diff_expr |= (arg == "--diff");
			eval_expr |= (arg == "--eval");
		} else if (arg == "--by") {
			if (++i >= argc)
				throw std::invalid_argument("No value specified for --by");
			diff_by = argv[i];
		} else if (arg.find("=") != std::string::npos) {
			auto pos = arg.find("=");
			std::string var_name = to_lower(arg.substr(0, pos)),
						val_str = arg.substr(pos + 1);
			contains_complex |= is_complex(val_str);
			try {
				std::smatch match;
				if (contains_complex &&
					std::regex_match(val_str, match, complex_regex)) {
					long double real_part = std::stold(match[1].str());
					long double imag_part = std::stold(match[2].str());
					complex_variables[var_name] =
						std::complex<long double>(real_part, imag_part);
				} else {
					variables[var_name] = std::stold(val_str);
				}
			} catch (...) {
				throw std::invalid_argument("Invalid value: " + val_str);
			}
		} else {
			throw std::invalid_argument("Unknown argument: " + arg);
		}
	}

	if (contains_complex) {
		auto expression =
			Derivative::Expression<std::complex<long double>>::from_string(
				expression_string
			);
		std::cout << run_task(
						 expression, diff_expr, eval_expr, diff_by,
						 complex_variables
					 )
				  << "\n";
	} else {
		auto expression =
			Derivative::Expression<long double>::from_string(expression_string);
		std::cout << run_task(
						 expression, diff_expr, eval_expr, diff_by, variables
					 )
				  << "\n";
	}
	return 0;
}
