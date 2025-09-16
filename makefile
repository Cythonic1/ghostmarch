



CXX     := g++
CXXFLAGS:= -Wall -Werror -std=c++23
SRC     := main.cpp
TARGET  := ghostmarch
PREFIX   ?= /usr
BINDIR   := $(PREFIX)/bin
.PHONY: all debug release clean

all: release

release: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(SRC) $(CXXFLAGS) -O3 -o $@

debug: $(TARGET)-debug

$(TARGET)-debug: $(SRC)
	$(CXX) $(SRC) $(CXXFLAGS) -g -O0 -DDEBUG -o $@

install: $(TARGET)
	install -d $(DESTDIR)$(BINDIR)
	install -m 755 $(TARGET) $(DESTDIR)$(BINDIR)
 
uninstall:
	rm -f $(DESTDIR)$(BINDIR)/$(TARGET)

clean:
	rm -f $(TARGET) $(TARGET)-debug
