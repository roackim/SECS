
# Windows Build
WTARGET = main.exe
WCXX = x86_64-w64-mingw32-gcc -Isrc -std=c++17
WBUILD_DIR = wobj

WLIB = -I/home/joackim/sfml-mingw/SFML-2.5.1/include -DSFML_STATIC -static
WLIB2 = -L /home/joackim/sfml-mingw/SFML-2.5.1/lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lsfml-audio-s -lsfml-network-s -lstdc++ -static

WDEPS_system = -lwinmm
WDEPS_graphics = -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype
WDEPS_window = -lsfml-system-s -lopengl32 -lwinmm -lgdi32
WDEPS_audio = -lsfml-system-s -lopenal32 -lvorbisenc -lvorbisfile -lvorbis -logg -lFLAC
WDEPS_network = -lsfml-system-s -lws2_32

WDEPS = $(WDEPS_system) $(WDEPS_graphics) $(WDEPS_window) $(WDEPS_network) $(WDEPS_audio)

# Structure
TARGET = main
BIN_DIR = bin
BUILD_DIR = obj
SRC_DIR = src

LIB = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network 

# C++ flags and options
CXX=g++ -Isrc -std=c++17 -fmax-errors=1# -O0 -g -pthread -lpthread -Wl,--no-as-needed
CPP_FLAGS ?= -pedantic -Werror -Wall -Wpedantic -Wextra -Wcast-align -Wunused -Wnull-dereference -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wnon-virtual-dtor -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wswitch-default -Wundef -Wno-unused
#				 	^-> warnigns are now errors
# Removed: -O3 -Wmissing-declarations -pedantic -Wsign-conversion -Wdouble-promotion -Wuseless-cast

# Files

SRCS := $(shell find $(SRC_DIR) -name *.cpp)
#OBJS_OLD := $(SRCS:%=$(BUILD_DIR)/%.o)

SRCS := $(SRCS:src/%=%)
OBJS := $(SRCS:.cpp=.o)

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
	
# windows version
.PHONY: wall
wall: $(BIN_DIR)/$(WTARGET)
	@echo \> Build complete

# windows version
.PHONY: wrun
wrun: wall
	@echo ------------------
	cd $(BIN_DIR) && wine $(WTARGET)

# ====[ WINDOWS target ]====
# Compiation
$(WBUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo \> Compiling..
	@$(MKDIR_P) $(dir $@)
	@$(WCXX) $(CPP_FLAGS) -c $< -o $@ $(WLIB)

# Linking
$(BIN_DIR)/$(WTARGET): $(OBJS:%=$(WBUILD_DIR)/%)
	@echo \> Linking..
	@$(MKDIR_P) $(dir $@)
	@$(WCXX) $(OBJS:%=$(WBUILD_DIR)/%) -o $@ $(WLIB2) $(WDEPS)

# ====[ LINUX target ]====
# Compilation
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo \> Compiling..
	@$(MKDIR_P) $(dir $@)
	@$(CXX) $(CPP_FLAGS) -c $< -o $@

# Linking
$(BIN_DIR)/$(TARGET): $(OBJS:%=$(BUILD_DIR)/%)
	@echo \> Linking..
	@$(MKDIR_P) $(dir $@)
	@$(CXX) $(OBJS:%=$(BUILD_DIR)/%) -o $@ $(LIB)

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

# -include $(DEPS)

