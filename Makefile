CXX ?= c++
CXXFLAGS ?= -Wall -Wextra -pedantic -O2 -std=c++20
DOXYGEN ?= doxygen
DOXYFILE ?= Doxyfile

BIN_DIR := bin
BUILD_DIR := build
INCLUDE_DIR := include
PROGRAM := $(BIN_DIR)/counterbenchmark
SOURCES := main.cpp $(wildcard src/*.cpp)
OBJECTS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)

CPPFLAGS += -I$(INCLUDE_DIR)
THREAD_FLAGS ?= -pthread

.PHONY: all clean doc docs

.DEFAULT_GOAL := all
all: $(PROGRAM)

$(PROGRAM): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) $(THREAD_FLAGS) -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(THREAD_FLAGS) -MMD -MP -MF $(@:.o=.d) -c $< -o $@

$(BIN_DIR):
	mkdir -p $@

-include $(DEPS)

doc docs: $(DOXYFILE)
	$(DOXYGEN) $(DOXYFILE)

clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR)
