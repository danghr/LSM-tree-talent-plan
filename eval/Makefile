# Compiler and flags
CXX = g++
CXXFLAGS = -g -Wall -Wextra -std=c++17

# LevelDB static library and headers
LEVELDB_LIB = ../build/libleveldb.a
LEVELDB_INCL = -I../include

# Source files and object files
SRCS = $(wildcard *.cc)
OBJS = $(SRCS:.cc=.o)

# Include directories
INCLUDES = $(LEVELDB_INCL)

# Task targets
TASK1 = task1
TASK2 = task2
TASK3 = task3

# List of all task targets
TARGET = $(TASK1)_build $(TASK2)_build $(TASK3)_build

# List of all source files excluding task files
OTHER_SRCS = $(filter-out task1.cc task2.cc task3.cc, $(SRCS))
OTHER_OBJS = $(OTHER_SRCS:.cc=.o)

# Object files for each task
TASK1_OBJS = task1.o $(OTHER_OBJS)
TASK2_OBJS = task2.o $(OTHER_OBJS)
TASK3_OBJS = task3.o $(OTHER_OBJS)

# Default target
all: $(TARGET)

# Targets
$(TASK1): $(TASK1)_build
	./$(TASK1)_build

$(TASK2): $(TASK2)_build
	./$(TASK2)_build

$(TASK3): $(TASK3)_build
	./$(TASK3)_build

$(TASK1)_build: $(TASK1_OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LEVELDB_LIB) -o $@

$(TASK2)_build: $(TASK2_OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LEVELDB_LIB) -o $@

$(TASK3)_build: $(TASK3_OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LEVELDB_LIB) -o $@

# Rule to compile source files to object files
%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean target
clean:
	rm -f $(TARGET) $(OBJS) *.log

# Phony targets
.PHONY: all clean
