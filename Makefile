CC = g++
CFLAGS = -std=c++11
TEST_FLAGS = -I.  # Include current directory for catch.hpp

# List of source files for your main program
MAIN_SRCS = findComp.cpp ConnectedComponent.cpp PGMimageProcessor.cpp

# List of object files for your main program
MAIN_OBJS = $(MAIN_SRCS:.cpp=.o)

# Target executable for your main program
MAIN_TARGET = findComp

# List of source files for your test file
TEST_SRCS = test_main.cpp ConnectedComponent.cpp PGMimageProcessor.cpp

# List of object files for your test file
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

# Target executable for your test file
TEST_TARGET = test

# Rule to compile and link your main program
$(MAIN_TARGET): $(MAIN_OBJS)
	$(CC) $(CFLAGS) $(MAIN_OBJS) -o $(MAIN_TARGET)

# Rule to compile and link your test file
$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CFLAGS) $(TEST_FLAGS) $(TEST_OBJS) -o $(TEST_TARGET)

# Rule to compile source files into object files for your main program
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile source files into object files for your test file
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove object files and the executables
clean:
	rm -f $(MAIN_OBJS) $(MAIN_TARGET) $(TEST_OBJS) $(TEST_TARGET)