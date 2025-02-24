#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace test {
class Tester {
  private:
	struct Test {
		std::function<bool()> function;
		std::string info_message;
		bool is_exception_test;
	};
	std::string group_name;
	uint32_t test_success;
	uint32_t tests_total;
	std::vector<Test> tests;

  public:
	Tester(std::string group_name);
	void register_test(
		std::function<bool()> test_function, const std::string &testMessage
	);
	void register_exception_test(
		std::function<void()> test_function, const std::string &testMessage
	);
	bool run_tests(void);
};
} // namespace test
