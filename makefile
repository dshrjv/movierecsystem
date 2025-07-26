CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Output binary name
TARGET = main

# Source files
SRCS = main.cpp user.cpp recommender.cpp utils.cpp data_loader.cpp experiments.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files to create final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile .cpp to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)