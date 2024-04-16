# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -O3

# Executable name
EXEC = ga

# Source files
SRCS = ga.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Header files
HEADERS = rubik.h

# Default target
all: $(EXEC)

# Rule to link object files and create the executable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Rule to compile source files into object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(EXEC)
