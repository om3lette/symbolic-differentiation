CC=g++
CFLAGS=-c --std=c++23 -Wall -Wextra -Werror -Wundef -pedantic
LDFLAGS=

BUILD_PATH=build
COVERAGE_PATH=coverage
SRC_PATH=src
TESTS_PATH=tests

EXPRESSIONS_IMPL = $(wildcard $(SRC_PATH)/expressions/*.cpp)
EXPRESSION_OUT_FILES = $(patsubst $(SRC_PATH)/expressions/%.cpp, $(BUILD_PATH)/%.o, $(EXPRESSIONS_IMPL))

BUILD ?= debug
DIGITS ?= 100

ifeq ($(BUILD), release)
	CFLAGS += -O3 -DNDEBUG
else
	CFLAGS += -g -Og --coverage
	LDFLAGS += --coverage
endif

COMPILE = $(CC) $(CFLAGS)
LINK = $(CC) $(LDFLAGS)

test: $(BUILD_PATH)/test-build
	$(BUILD_PATH)/test-build

test.build: tests.o $(BUILD_PATH)/tester.o $(BUILD_PATH)/lexer.o parser.o $(EXPRESSION_OUT_FILES) | $(BUILD_PATH)
	$(LINK) $(BUILD_PATH)/tests.o $(BUILD_PATH)/tester.o $(BUILD_PATH)/lexer.o $(BUILD_PATH)/parser.o $(EXPRESSION_OUT_FILES) -o $(BUILD_PATH)/test-build

executable.o: $(EXPRESSION_OUT_FILES) | $(BUILD_PATH)
	$(LINK) $^ -o $(BUILD_PATH)/executable

parser.o: $(SRC_PATH)/parser/Parser.cpp | $(BUILD_PATH)
	$(COMPILE) $(SRC_PATH)/parser/Parser.cpp -o $(BUILD_PATH)/parser.o

$(BUILD_PATH)/lexer.o: $(SRC_PATH)/parser/Lexer.cpp | $(BUILD_PATH)
	$(COMPILE) $(SRC_PATH)/parser/Lexer.cpp -o $(BUILD_PATH)/lexer.o

$(BUILD_PATH)/%.o: $(SRC_PATH)/expressions/%.cpp | $(BUILD_PATH)
	$(COMPILE) -c $< -o $@

tests.o: $(TESTS_PATH)/tests.cpp | $(BUILD_PATH)
	$(COMPILE) $(TESTS_PATH)/tests.cpp -o $(BUILD_PATH)/tests.o

$(BUILD_PATH)/tester.o: $(TESTS_PATH)/Tester.cpp | $(BUILD_PATH)
	$(COMPILE) $(TESTS_PATH)/Tester.cpp -o $(BUILD_PATH)/tester.o

$(BUILD_PATH):
	mkdir -p $(BUILD_PATH)

clean:
	rm -rf $(BUILD_PATH) $(COVERAGE_PATH)

.PHONY: all clean $(BUILD_PATH)
