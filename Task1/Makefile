# Compiler and flags
CXX = g++
CXXFLAGS = -g -Wall -Wextra -std=c++17

# Target executable
TARGET = task1

# LevelDB static library and headers
LEVELDB_LIB = ../build/libleveldb.a
LEVELDB_INCL = -I../include

# Source files and object files
SRCS = $(wildcard *.cc)
OBJS = $(SRCS:.cc=.o)

# Include directories
INCLUDES = $(LEVELDB_INCL)

# Default target
all: $(TARGET)

# Run the executable
run: $(TARGET)
	./$(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LEVELDB_LIB)

# Rule to compile source files to object files
%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean target
clean:
	rm -f $(TARGET) $(OBJS) *.log

# Phony targets
.PHONY: all clean
