# Define the compiler
CXX = g++

# Define compiler flags
LDFLAGS = -L/opt/homebrew/opt/libpqxx/lib -L/opt/homebrew/opt/postgresql@14/lib/postgresql@14 -I/opt/homebrew/opt/boost/include -I/Users/sindri/Desktop/cpp-chatroom/websocketpp -lpqxx -lpq -L/opt/homebrew/opt/ncurses/lib -lncurses -pthread
CXXFLAGS = -Wall -std=c++17 -Iinclude/ -I/opt/homebrew/opt/libpqxx/include -I/opt/homebrew/opt/postgresql@14/include -DNCURSES_NOMACROS -I/opt/homebrew/opt/boost/include -I/opt/homebrew/opt/ncurses/include -I/Users/sindri/Desktop/cpp-chatroom/websocketpp

PROJECT_ROOT_DIR = ./
SRC_DIR = src/
INCLUDE_DIR = include/
BUILD_DIR = build/

# Define source files and object files
SOURCES = $(wildcard $(SRC_DIR)*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)%.cpp=$(BUILD_DIR)%.o)

# Define the target executable
TARGET = $(PROJECT_ROOT_DIR)a

# Define the rule for building the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

# Define the rule for compiling source files to object files
$(BUILD_DIR)%.o: $(SRC_DIR)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Define the clean rule to remove object files and the executable
clean:
	rm -f $(OBJECTS) $(TARGET)
