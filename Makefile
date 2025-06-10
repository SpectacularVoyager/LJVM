CXX=g++

SRC_FILES=$(shell find -wholename "./src/*.cpp")

OBJECT_FILES=$(SRC_FILES:./src/%.cpp=out/%.o)

BUILD_OUTPUT=build/app

COLOUR_GREEN=\033[0;32m
COLOUR_RED=\033[0;31m
COLOUR_BLUE=\033[0;34m
END_COLOUR=\033[0m

all:$(BUILD_OUTPUT)
	@mkdir -p out
	@mkdir -p build

$(BUILD_OUTPUT): $(OBJECT_FILES)
	@$(CXX) -o build/app $(OBJECT_FILES)
	@echo "\033[0;34mBUILDING\033[0m" $<

$(OBJECT_FILES):out/%.o: src/%.cpp
	@$(CXX) -c -o $@ $<
	@echo "\033[0;32mCOMPILING\033[0m" $<

