CXX      ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra
CXXFLAGS += -Iinclude -MMD -MP

SRCS := $(wildcard src/*.cpp)
OBJS := $(SRCS:src/%.cpp=build/%.o)

raytracer: $(OBJS)
	$(CXX) $^ -o build/$@

build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

TEST_SRCS := $(wildcard tests/*.cpp)
TEST_BINS := $(TEST_SRCS:tests/%.cpp=build/%)
LIB_OBJS  := $(filter-out build/main.o,$(OBJS))

test: $(TEST_BINS)
	@set -e; for t in $^; do echo "== $$t"; ./$$t; done

build/test_%: tests/test_%.cpp $(LIB_OBJS)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -rf build

.PHONY: clean test
-include $(OBJS:.o=.d)
