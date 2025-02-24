CC=g++
CFLAGS=-c --std=c++23 -Wall -Wextra -Werror -Wundef -pedantic
LDFLAGS=

BUILD_PATH=build
COVERAGE_PATH=coverage
SRC_PATH=src
TESTS_PATH=tests

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

test.build: link-tests

$(BUILD_PATH):
	mkdir -p $(BUILD_PATH)

link-tests: tests.o tester.o lexer.o parser.o | $(BUILD_PATH)
	$(LINK) $(BUILD_PATH)/tests.o $(BUILD_PATH)/tester.o $(BUILD_PATH)/lexer.o $(BUILD_PATH)/parser.o -o $(BUILD_PATH)/test-build

parser.o: $(SRC_PATH)/parser/Parser.cpp | $(BUILD_PATH)
	$(COMPILE) $(SRC_PATH)/parser/Parser.cpp -o $(BUILD_PATH)/parser.o

lexer.o: $(SRC_PATH)/parser/Lexer.cpp | $(BUILD_PATH)
	$(COMPILE) $(SRC_PATH)/parser/Lexer.cpp -o $(BUILD_PATH)/lexer.o

tests.o: $(TESTS_PATH)/tests.cpp | $(BUILD_PATH)
	$(COMPILE) $(TESTS_PATH)/tests.cpp -o $(BUILD_PATH)/tests.o

tester.o: $(TESTS_PATH)/Tester.cpp | $(BUILD_PATH)
	$(COMPILE) $(TESTS_PATH)/Tester.cpp -o $(BUILD_PATH)/tester.o

clean:
	rm -rf $(BUILD_PATH) $(COVERAGE_PATH)