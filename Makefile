CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
INCLUDES = -I./src

# Output binary name
TARGET = opus

# Source directories
SRC_DIR = src
AUTH_DIR = $(SRC_DIR)/authentication

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(AUTH_DIR)/*.cpp)
OBJS = $(SRCS:.cpp=.o)

# Header files for dependency tracking
DEPS = $(SRCS:.cpp=.d)

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Compilation with dependency generation
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# Include dependency files
-include $(DEPS)

# Clean
clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

# Run
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
