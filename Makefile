# flags
CXX=g++
RM=rm -r
CPPFLAGS=-std=c++11

# dirs
BUILD=build
SRC=src

# src files
FILES = bfi.cpp
SOURCES = $(FILES:%.cpp=$(SRC)/%.cpp)


default: bfi

bfi:
	mkdir $(BUILD)
	$(CXX) $(CPPFLAGS) $(SOURCES) -o $(BUILD)/bfi.o

clean:
	$(RM) $(BUILD)
