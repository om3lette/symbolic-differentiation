CC=g++
CFLAGS=-c --std=c++23 -Wall -Wextra -Werror -Wundef -pedantic
LDFLAGS=

BASE_BUILD_PATH=build
BUILD_PATH=$(BASE_BUILD_PATH)
COVERAGE_PATH=coverage
SRC_PATH=src
TESTS_PATH=tests

EXPRESSIONS_IMPL = $(wildcard $(SRC_PATH)/expressions/*.cpp)
EXPRESSION_OUT_FILES = $(patsubst $(SRC_PATH)/expressions/%.cpp, $(BUILD_PATH)/%.o, $(EXPRESSIONS_IMPL))

TEST_GROUPS_IMPL = $(wildcard $(TESTS_PATH)/groups/*.cpp)
TEST_GROUPS_OUT_FILES = $(patsubst $(TESTS_PATH)/groups/%.cpp, $(BUILD_PATH)/%.o, $(TEST_GROUPS_IMPL))

BUILD ?= debug

ifeq ($(BUILD), release)
	CFLAGS += -O3 -DNDEBUG
	BUILD_PATH = build/release
else
	CFLAGS += -g -Og --coverage
	LDFLAGS += --coverage
	BUILD_PATH = build/debug
endif

COMPILE = $(CC) $(CFLAGS)
LINK = $(CC) $(LDFLAGS)

test: $(BUILD_PATH)/test-build
	$(BUILD_PATH)/test-build

test.build: $(BUILD_PATH)/tester.o $(BUILD_PATH)/lexer.o $(BUILD_PATH)/parser.o $(EXPRESSION_OUT_FILES) $(TEST_GROUPS_OUT_FILES) $(BUILD_PATH)/tests.o | $(BUILD_PATH)
	$(LINK) $(BUILD_PATH)/tester.o $(BUILD_PATH)/lexer.o $(BUILD_PATH)/parser.o $(EXPRESSION_OUT_FILES) $(TEST_GROUPS_OUT_FILES) $(BUILD_PATH)/tests.o -o $(BUILD_PATH)/test-build

console-build: $(BUILD_PATH)/lexer.o $(BUILD_PATH)/parser.o $(BUILD_PATH)/console.o $(EXPRESSION_OUT_FILES)  | $(BUILD_PATH)
	$(LINK) $^ -o $(BUILD_PATH)/differentiator

$(BUILD_PATH)/console.o: $(SRC_PATH)/console.cpp | $(BUILD_PATH)
	$(COMPILE) $(SRC_PATH)/console.cpp -o $(BUILD_PATH)/console.o

$(BUILD_PATH)/parser.o: $(SRC_PATH)/parser/Parser.cpp | $(BUILD_PATH)
	$(COMPILE) $(SRC_PATH)/parser/Parser.cpp -o $(BUILD_PATH)/parser.o

$(BUILD_PATH)/lexer.o: $(SRC_PATH)/parser/Lexer.cpp | $(BUILD_PATH)
	$(COMPILE) $(SRC_PATH)/parser/Lexer.cpp -o $(BUILD_PATH)/lexer.o

$(BUILD_PATH)/%.o: $(SRC_PATH)/expressions/%.cpp | $(BUILD_PATH)
	$(COMPILE) -c $< -o $@

$(BUILD_PATH)/%.o: $(TESTS_PATH)/groups/%.cpp | $(BUILD_PATH)
	$(COMPILE) -c $< -o $@

$(BUILD_PATH)/tests.o: $(TESTS_PATH)/tests.cpp | $(BUILD_PATH)
	$(COMPILE) $(TESTS_PATH)/tests.cpp -o $(BUILD_PATH)/tests.o

$(BUILD_PATH)/tester.o: $(TESTS_PATH)/Tester.cpp | $(BUILD_PATH)
	$(COMPILE) $(TESTS_PATH)/Tester.cpp -o $(BUILD_PATH)/tester.o

$(BUILD_PATH):
	mkdir -p $(BUILD_PATH)

clean:
	rm -rf $(BASE_BUILD_PATH) $(COVERAGE_PATH)

.PHONY: all clean $(BUILD_PATH)
