CXX = g++
CXXFLAGS = -Wall -std=c++17 -O2
INCLUDES = -Iinclude

# Directory structure
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Source files
SOURCES = $(SRCDIR)/scheduling.cpp $(SRCDIR)/main_scheduling.cpp $(SRCDIR)/accounts.cpp
TEST_SOURCES = $(SRCDIR)/scheduling.cpp test/test.cpp $(SRCDIR)/accounts.cpp

# Object files
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
TEST_OBJECTS = $(TEST_SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Executables
MAIN = $(BINDIR)/scheduler
TEST = $(BINDIR)/test

# Default target
all: directories $(MAIN) $(TEST)

# Create directories if they don't exist
directories:
	mkdir -p $(OBJDIR) $(BINDIR)

# Build main program
$(MAIN): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build test program
$(TEST): $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compile test source files
$(OBJDIR)/test.o: $(SRCDIR)/test.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Run tests
test: $(TEST)
	./$(TEST)

# Run with different workloads
run: $(MAIN)
	@echo "\n=== Simple Workload ===\n"
	./$(MAIN) workloads/workload_01.txt
	@echo "\n=== Fee Priority Workload ===\n"
	./$(MAIN) workloads/workload_02.txt
	@echo "\n=== Time Priority Workload ===\n"
	./$(MAIN) workloads/workload_03.txt
	@echo "\n=== Account Tier Workload ===\n"
	./$(MAIN) workloads/workload_04.txt
	@echo "\n=== Complex Mixed Workload ===\n"
	./$(MAIN) workloads/workload_05.txt

# Run with specific workload for easier testing
run1: $(MAIN)
	./$(MAIN) workloads/workload_01.txt

run2: $(MAIN)
	./$(MAIN) workloads/workload_02.txt

run3: $(MAIN)
	./$(MAIN) workloads/workload_03.txt

run4: $(MAIN)
	./$(MAIN) workloads/workload_04.txt

run5: $(MAIN)
	./$(MAIN) workloads/workload_05.txt

# Clean build files
clean:
	rm -rf $(OBJDIR)/* $(BINDIR)/*

.PHONY: all directories test run run1 run2 run3 run4 run5 clean