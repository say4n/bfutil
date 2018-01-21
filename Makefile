# flags
CXX=g++
RM=rm -r
CPPFLAGS=-std=c++11

# dirs
BUILD=build
SRC=src

# src files
bfi_FILES = bfi.cpp
bfi_SOURCES = $(bfi_FILES:%.cpp=$(SRC)/%.cpp)

bf2c_FILES = bf2c.cpp
bf2c_SOURCES = $(bf2c_FILES:%.cpp=$(SRC)/%.cpp)

default: bfi bf2c

bfi:
	mkdir -p $(BUILD)
	$(CXX) $(CPPFLAGS) $(bfi_SOURCES) -o $(BUILD)/bfi.o

bf2c:
	mkdir -p $(BUILD)
	$(CXX) $(CPPFLAGS) $(bf2c_SOURCES) -o $(BUILD)/bf2c.o

clean:
	$(RM) $(BUILD)
