
BUILD_DIR ?= build
ARCHS = -arch arm64 # -arch x86_64
INCLUDES = -I./include
CC = clang++
CCFLAGS += -std=c++17 -finput-charset=utf-8 -fdiagnostics-show-template-tree -fno-elide-type -g $(ARCHS) $(INCLUDES)
LD = clang++
LDFLAGS += $(ARCHS)
JTEST_HEADERS := $(wildcard include/JTest/*.h)
JTEST_SOURCES := $(wildcard src/JTest/*.cpp)
JTEST_OBJECTS := $(patsubst src/%.cpp, $(BUILD_DIR)/%.o, $(JTEST_SOURCES))
#$(patsubst src/%.cpp, $(BUILD_DIR)/%.o, $(JTEST_SOURCES))

.Phony: clean all run

all: $(BUILD_DIR)/examples/example

clean:
	rm -rf $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/examples
	mkdir -p $(BUILD_DIR)/JTest
# @echo JTEST_HEADERS = $(JTEST_HEADERS)
# @echo JTEST_SOURCES = $(JTEST_SOURCES)
# @echo JTEST_OBJECTS = $(JTEST_OBJECTS)

run: $(BUILD_DIR)/examples/example
	$(BUILD_DIR)/examples/example

$(BUILD_DIR)/examples/example: $(BUILD_DIR)/examples/example.o $(JTEST_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/examples/%.o:  examples/%.cpp examples/ClassToTest.h $(JTEST_HEADERS)
	$(CC) $(CCFLAGS) -c -o $@ $<

$(BUILD_DIR)/examples/JTest.o: examples/JTest.cpp examples/JTest.h $(JTEST_HEADERS)
	$(CC) $(CCFLAGS) -c -o $@ $<

$(BUILD_DIR)/JTest/%.o: src/JTest/%.cpp $(JTEST_HEADERS)
	$(CC) $(CCFLAGS) -c -o $@ $<
