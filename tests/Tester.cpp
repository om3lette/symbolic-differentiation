#include "Tester.hpp"
#include <iomanip>
#include <iostream>

namespace test {
Tester::Tester(std::string group_name)
	: group_name(std::move(group_name)), test_success(0), tests_total(0),
	  tests(std::vector<Test>()) {}

void Tester::register_test(
	std::function<bool()> test_function, const std::string &testMessage
) {
	tests.push_back({test_function, testMessage, false});
	tests_total++;
}

void Tester::register_exception_test(
	std::function<void()> test_function, const std::string &test_message
) {
	// Make it return bool;
	auto wrapped_function = [test_function]() -> bool {
		test_function();
		return true;
	};
	tests.push_back({wrapped_function, test_message, true});
	tests_total++;
}

bool Tester::run_tests(void) {
	std::cout << '\n'
			  << "=========== Running [" << group_name << ']'
			  << " ===========\n";
	if (tests.size() == 0) {
		std::cout << "\n\033[1;33m[WARNING]\033[0m No tests were found. Please "
					 "register some "
					 "before running.\n";
		return true;
	}
	for (size_t i = 0; i < tests.size(); i++) {
		Test test = tests[i];
		bool success = false;
		if (!test.is_exception_test)
			success = test.function();
		else {
			try {
				test.function();
			} catch (...) {
				success = true;
			}
		}
		test_success += success;
		std::cout << i + 1 << ") " << test.info_message << ": "
				  << (success ? "\033[1;32m[OK]\033[0m"
							  : "\033[1;31m[FAIL]\033[0m")
				  << '\n';
	}
	float correctPercentage =
		(static_cast<float>(test_success) / tests_total) * 100;
	// TODO
	// std::cout << std::setprecision(2) << std::fixed;
	int color = 32; // Green
	if (test_success == 0)
		color = 31; // Red
	else if (test_success < tests_total)
		color = 33; // Yellow
	std::cout << "\033[1;" << color
			  << "m\nTest results: " << std::to_string(test_success) << '/'
			  << std::to_string(tests_total) << ' ' << correctPercentage
			  << "%\033[0m" << std::endl;
	return tests_total == test_success;
}
} // namespace test
