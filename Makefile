CXX=g++

SRC_FILES=$(shell find -wholename "./src/*.cpp")

OBJECT_FILES=$(SRC_FILES:./src/%.cpp=out/%.o)

LIB_SRC=src/jni/jni.cpp

LIB_OUT_FILES=$(LIB_SRC:./src/%.cpp=out/%.o)

BUILD_OUTPUT=build/app

LIBRARY_OUTPUT=lib/jvm.so

COLOUR_GREEN=\033[0;32m
COLOUR_RED=\033[0;31m
COLOUR_BLUE=\033[0;34m
END_COLOUR=\033[0m

all:$(BUILD_OUTPUT) lib

$(BUILD_OUTPUT): $(OBJECT_FILES)
	@mkdir -p $(shell dirname $@)
	@$(CXX) -o build/app $(OBJECT_FILES)
	@echo "\033[0;34mBUILDING\t\033[0m" $@

$(OBJECT_FILES):out/%.o: src/%.cpp
	@mkdir -p $(shell dirname $@)
	@$(CXX) -c -o $@ $< -Isrc
	@echo "\033[0;32mCOMPILING\t\033[0m" $<

clean:
	@echo "\033[0;33mCLEANING\t\033[0m"
	@rm -rf out build lib

lib:$(LIBRARY_OUTPUT)

$(LIBRARY_OUTPUT): $(LIB_OUT_FILES)
	@mkdir -p $(shell dirname $@)
	@$(CXX) -fpic -shared -o $(LIBRARY_OUTPUT) $(LIB_OUT_FILES)
	@echo "\033[0;36mCREATING LIBRARY\033[0m" $@
