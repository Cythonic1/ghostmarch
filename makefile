



CXX     := g++
CXXFLAGS:= -Wall -Werror -std=c++23
SRC     := main.cpp
TARGET  := main

.PHONY: all debug release clean

all: release

release: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(SRC) $(CXXFLAGS) -O3 -o $@

debug: $(TARGET)-debug

$(TARGET)-debug: $(SRC)
	$(CXX) $(SRC) $(CXXFLAGS) -g -O0 -DDEBUG -o $@

clean:
	rm -f $(TARGET) $(TARGET)-debug
