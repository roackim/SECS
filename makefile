
# +-------------------------+
# |       MAIN SETUP        |
# +-------------------------+

# Structure
TARGET    := main
BIN_DIR   := bin
BUILD_DIR := obj
SRC_DIR   := src

# C++ flags and options
CXX := g++ -Isrc -std=c++17 -fmax-errors=1# -O0 -g -pthread -lpthread -Wl,--no-as-needed
CPP_FLAGS := -pedantic -Werror -Wall -Wpedantic -Wextra -Wcast-align -Wunused -Wnull-dereference -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wnon-virtual-dtor -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wswitch-default -Wundef -Wno-unused
#				 	^-> warnigns are now errors
# Removed: -O3 -Wmissing-declarations -pedantic -Wsign-conversion -Wdouble-promotion -Wuseless-cast

SRCS := $(shell find $(SRC_DIR) -name *.cpp)
SRCS := $(SRCS:src/%=%)

OBJS := $(SRCS:.cpp=.o)
OBJS := $(filter-out main.o, $(OBJS)) # remove main -> used for tests target

MKDIR_P ?= mkdir -p
CPP_FLAGS := $(CPP_FLAGS)

# +-------------------------+
# |       ALL TARGET        |
# +-------------------------+

# Rule for Target executable
.PHONY: all
all: $(BIN_DIR)/$(TARGET)
	@echo \> Build complete

# Rule to run the executable
.PHONY: run
run: all
	@echo ------------------
	@cd $(BIN_DIR) && ./$(TARGET)

# ====[ LINUX target ]====
# Compilation
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo \> Compiling..
	@$(MKDIR_P) $(dir $@)
	@$(CXX) $(CPP_FLAGS) -c $< -o $@  -MMD -MP
	
# Compilation for main.cpp file
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp
	@echo \> Compiling..
	@$(MKDIR_P) $(dir $@)
	@$(CXX) $(CPP_FLAGS) -c $< -o $@  -MMD -MP

# Linking
$(BIN_DIR)/$(TARGET): $(OBJS:%=$(BUILD_DIR)/%) $(BUILD_DIR)/main.o
	@echo \> Building main..
	@$(MKDIR_P) $(dir $@)
	@$(CXX) $(OBJS:%=$(BUILD_DIR)/%) $(BUILD_DIR)/main.o -o $@ $(LIB)

# Dependencies
DEP_FILES := $(SRCS:%.cpp=$(BUILD_DIR)/%.d)
$(DEP_FILES):
include $(wildcard $(DEP_FILES))

# +-------------------------+
# |       UNIT TESTS        |
# +-------------------------+

TESTS_DIR := tests
TESTS_BUILD_DIR := tests_obj

TESTS_SRCS := $(shell find $(TESTS_DIR) -name *.cpp)
TESTS_SRCS := $(TESTS_SRCS:tests/%=%)
TESTS_OBJS := $(TESTS_SRCS:.cpp=.o)

.PHONY: test_all
test_all: $(TESTS_BUILD_DIR)/$(TARGET)
	@echo \> Test Build complete

.PHONY: test
test: test_all 
	@echo ------------------
	@cd $(TESTS_BUILD_DIR) && ./$(TARGET) -ni -nv
	
# Compilation to object files
$(TESTS_BUILD_DIR)/%.o: $(TESTS_DIR)/%.cpp
	@echo \> Compiling..
	@$(MKDIR_P) $(dir $@)
	@$(CXX) $(CPP_FLAGS) -Itests $(CPP_FLAGS) -c $< -o $@  -MMD -MP

# linking
$(TESTS_BUILD_DIR)/$(TARGET): $(OBJS:%=$(BUILD_DIR)/%) $(TESTS_OBJS:%=$(TESTS_BUILD_DIR)/%)
	@echo \> Building test..
	@$(MKDIR_P) $(dir $@)
	@$(CXX) $(CPP_FLAGS) -Itests $(OBJS:%=$(BUILD_DIR)/%) $(TESTS_OBJS:%=$(TESTS_BUILD_DIR)/%) -o $@ $(LIB)


# Tests Dependencies
TESTS_DEP_FILES := $(TESTS_SRCS:%.cpp=$(TESTS_BUILD_DIR)/%.d)
$(TESTS_DEP_FILES):
include $(wildcard $(TESTS_DEP_FILES))

# +-------------------------+
# |      OTHER TARGETS      |
# +-------------------------+

.PHONY: clean
clean:
	@$(RM) -rf $(BUILD_DIR)
	@$(RM) -rf $(WBUILD_DIR)
	@$(RM) -f $(BIN_DIR)/$(TARGET)
	@$(RM) -rf $(TESTS_BUILD_DIR)
	@$(RM) -rf $(BIN_DIR)
	
	@echo \> Cleaning..

.PHONY: valgrind
valgrind: all
#	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s --log-file=valgrind-out.txt ./bin/main
#	@valgrind --leak-check=full --tool=callgrind ./bin/main
	@cd bin && valgrind --tool=memcheck --leak-check=full ./main

.PHONY:
todo:
	@echo
	@cat TODO.txt | grep -F '[ ]'
	@echo

