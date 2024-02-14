
CURRENT_DIR = $(shell pwd)

# /// COMPILER  \\\ #
# Explicitly set compiler to avoid using default
CC := g++ # change to gcc if want gnu C compiler

# Set compiler flags
CFLAGS := -Wall 
# -Wall: enable a set of warnings, but actually not all.
# -W: enable extra warnings. It's advised to use -Wextra instead which has the same meaning
# -Werror: every warning is treated as an error.

INC := -Iinclude -Iheaders # -InameOfHeaderFolder
LIB := -Llib	# -LnameOfLibraryFolder
LDL := -lglfw -ldl -lGL # Linker Flags ex. -lm
# ///  \\\ #

# /// FILES  \\\ #
#List name of source, object & build output directories:
SRC_DIR := sources
OBJ_DIR := objects
BIN_DIR := . # buildDir or . if you want executable in the current directory

#Name your final target ie. executable
EXE := main # $(BIN_DIR)/main # <<< Need to change write permissions to use binary folder???<<<
# ///  \\\ #

# /// SOURCES \\\ #
# Wildcard function to get a string of all files with .c file extension in SRC_DIR folder
C_SRC := $(wildcard $(SRC_DIR)/*.c) 

# Wildcard function to get a string of all files with .cpp file extension in SRC_DIR folder
CPP_SRC := $(wildcard $(SRC_DIR)/*.cpp) 

# String concatenation of all source files
SRCS := $(C_SRC) $(CPP_SRC)
# ///  \\\ #

# /// OBJECTS  \\\ #
# Pattern matches first arg, and replaces with seconds arg all matches in third arg
C_OBJ := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(C_SRC))

CPP_OBJ := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(CPP_SRC))

# String concatenation of all object files
OBJS := $(C_OBJ) $(CPP_OBJ)
# ///  \\\ #

# /// RULES  \\\ #
# 1. Must use tabs not spaces
# 2. Automatic Variables all start with $	ex. $@, $^, etc.

#Make will think we want to actually create a file or folder named all, so let's say it's not a real target:
.PHONY: say all clean

#Convention that the default target should be called all and that it should be the first target in your Makefile
all: $(EXE) 

# Rule for main executable (Target : Prerequisites \ Recipe) 
#Linking step for all object files with linker flags
$(EXE) : $(OBJS) | $(BIN_DIR) 
	$(CC) -o $@ $^ $(LDL) 

# Rule for C sources compilation (Target : Prerequisites \ Recipe) 
# Compiles to object files the .c files found in source directory, but doesn't link
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -o $@ -c $< 

# Rule for CPP sources compilation (Target : Prerequisites \ Recipe) 
# Compiles to object files the .cpp files found in source directory, but doesn't link
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -o $@ -c $< 

# Rule to create Binary directory and/or Object directory, if they don't already exist.
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@
# -p, --parents :: No error if existing, make parent directories as needed

# Rule to delete binary and object directories, as well as executable
clean:
	@$(RM) $(EXE)
	@$(RM) -r $(BIN_DIR) $(OBJ_DIR)
# Note: Implicit Rule: $(RM) == rm -f 
# run make -p to see list of implicit rules
# @ disables the echoing of the command

# Rule to print desired output
say:
	@echo $(CURRENT_DIR)
# @ disables the echoing of the command

# /// END \\\ #
