
# +-------------------------+
# |       MAIN SETUP        |
# +-------------------------+

# Structure
TARGET = main
BIN_DIR = bin
BUILD_DIR = obj
SRC_DIR = src

TESTS_DIR = tests


# C++ flags and options
CXX=g++ -Isrc -std=c++17 -fmax-errors=1# -O0 -g -pthread -lpthread -Wl,--no-as-needed
CPP_FLAGS ?= -pedantic -Werror -Wall -Wpedantic -Wextra -Wcast-align -Wunused -Wnull-dereference -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wnon-virtual-dtor -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wswitch-default -Wundef -Wno-unused
#				 	^-> warnigns are now errors
# Removed: -O3 -Wmissing-declarations -pedantic -Wsign-conversion -Wdouble-promotion -Wuseless-cast

# Files

SRCS := $(shell find $(SRC_DIR) -name *.cpp)
SRCS := $(SRCS:src/%=%)
OBJS := $(SRCS:.cpp=.o)
OBJS := $(filter-out main.o, $(OBJS)) # remove main -> used for tests target

# Commands
MKDIR_P ?= mkdir -p
CPP_FLAGS ?= $(CPP_FLAGS) -MMD -MP

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
	@echo \> Compiling sources..
	@$(MKDIR_P) $(dir $@)
	@$(CXX) $(CPP_FLAGS) -c $< -o $@
	
# Compilation for main.cpp file
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp
	@echo \> Compiling main source..
	@$(MKDIR_P) $(dir $@)
	@$(CXX) $(CPP_FLAGS) -c $< -o $@

# Linking
$(BIN_DIR)/$(TARGET): $(OBJS:%=$(BUILD_DIR)/%) $(BUILD_DIR)/main.o
	@echo \> Building main..
	@$(MKDIR_P) $(dir $@)
	$(CXX) $(OBJS:%=$(BUILD_DIR)/%) $(BUILD_DIR)/main.o -o $@ $(LIB)
	
# +-------------------------+
# |       UNIT TESTS        |
# +-------------------------+

.PHONY: test_all
test_all: $(TESTS_DIR)/$(TARGET)
	@echo \> Test Build complete

.PHONY: test
test: test_all 
	@echo ------------------
	@cd $(TESTS_DIR) && ./$(TARGET) -ni -nv
	
TESTS_SRCS := $(shell find $(TESTS_DIR) -name *.cpp)

$(TESTS_DIR)/$(TARGET): $(OBJS:%=$(BUILD_DIR)/%) $(TESTS_SRCS)
	@echo \> Building test..
	@$(MKDIR_P) $(dir $@)
	@$(CXX) $(OBJS:%=$(BUILD_DIR)/%) $(TESTS_SRCS) -o $@ $(LIB)


# +-------------------------+
# |      OTHER TARGETS      |
# +-------------------------+

.PHONY: clean
clean:
	@$(RM) -rf $(BUILD_DIR)
	@$(RM) -rf $(WBUILD_DIR)
	@$(RM) -f $(BIN_DIR)/$(TARGET)
	@$(RM) -f $(TESTS_DIR)/$(TARGET)
	@echo \> Cleaning..

.PHONY: valgrind
valgrind: all
#	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s --log-file=valgrind-out.txt ./bin/main
#	@valgrind --leak-check=full --tool=callgrind ./bin/main
	@cd bin && valgrind --tool=memcheck --leak-check=full ./main

.PHONY:
todo:
	@echo
	@cat TODO.txt | grep '\[ \]'
	@echo

