
BUILD_DIR ?= build
ARCHS = -arch arm64 # -arch x86_64
CC = clang++
CCFLAGS += -std=c++17 -finput-charset=utf-8 -fdiagnostics-show-template-tree -fno-elide-type -g $(ARCHS) --stdlib=libc++
LD = clang++
LDFLAGS += $(ARCHS)

.Phony: clean all $(BUILD_DIR)/test.txt

all: $(BUILD_DIR)/test.txt $(BUILD_DIR)/examples/example
	@echo $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/examples
	@echo CC=$(CC)
	@echo CCFLAGS=$(CCFLAGS)
	@echo LD=$(LD)
	@echo LDFLAGS=$(LDFLAGS)

$(BUILD_DIR)/test.txt:
	@echo $(BUILD_DIR)/test.txt

$(BUILD_DIR)/examples/example: $(BUILD_DIR)/examples/example.o $(BUILD_DIR)/examples/JTest.o
	$(LD) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/examples/example.o:  examples/example.cpp examples/ClassToTest.h examples/JTest.h
	$(CC) $(CCFLAGS) -c -o $@ $<

$(BUILD_DIR)/examples/JTest.o: examples/JTest.cpp examples/JTest.h
	$(CC) $(CCFLAGS) -c -o $@ $<
