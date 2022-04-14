

# Structure
TARGET = main
BIN_DIR = bin
BUILD_DIR = obj
SRC_DIR = src


# C++ flags and options
CXX=g++ -Isrc -std=c++17 -fmax-errors=1# -O0 -g -pthread -lpthread -Wl,--no-as-needed
CPP_FLAGS ?= -pedantic -Werror -Wall -Wpedantic -Wextra -Wcast-align -Wunused -Wnull-dereference -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wnon-virtual-dtor -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wswitch-default -Wundef -Wno-unused
#				 	^-> warnigns are now errors
# Removed: -O3 -Wmissing-declarations -pedantic -Wsign-conversion -Wdouble-promotion -Wuseless-cast

SRCS := $(shell find $(SRC_DIR) -name *.cpp)
#OBJS_OLD := $(SRCS:%=$(BUILD_DIR)/%.o)

SRCS := $(SRCS:src/%=%)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)ma

# Commands
MKDIR_P ?= mkdir -p
CPP_FLAGS ?= $(CPP_FLAGS) -MMD -MP

# Rule for Target executable
.PHONY: all
all: $(BIN_DIR)/$(TARGET)
	@echo \> Build complete

# Rule to run the executable
.PHONY: run
run: all
	@echo ------------------
	cd $(BIN_DIR) && ./$(TARGET)

# ====[ LINUX target ]====
# Compilation
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp makefile 
	@echo \> Compiling..
	@$(MKDIR_P) $(dir $@)
	@$(CXX) $(CPP_FLAGS) -c $< -o $@

# Linking
$(BIN_DIR)/$(TARGET): $(OBJS:%=$(BUILD_DIR)/%)
	@echo \> Linking..
	@$(MKDIR_P) $(dir $@)
	@$(CXX) $(OBJS:%=$(BUILD_DIR)/%) -o $@ $(LIB)
	
.PHONY: depend
depend: $(SRCS)
	@$(RM) -f ./.depend
	@$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

.PHONY: clean
clean:
	@$(RM) -r $(BUILD_DIR)
	@$(RM) -r $(WBUILD_DIR)
	@$(RM) $(BIN_DIR)/$(TARGET)
	@echo \> Cleaning..

.PHONY: valgrind
valgrind: all
#	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s --log-file=valgrind-out.txt ./bin/main
#	@valgrind --leak-check=full --tool=callgrind ./bin/main
	@cd bin && valgrind --tool=memcheck --leak-check=full ./main

.PHONY:
todo:
	@echo
	@cat TODO.txt | grep -F [ ]
	@echo

-include $(DEPS)