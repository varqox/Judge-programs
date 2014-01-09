# Compile commands
ifeq ($(shell clang > /dev/null 2> /dev/null; echo $$?), $(shell echo 1))
export CC = clang
else
export CC = gcc
endif
ifeq ($(shell clang++ > /dev/null 2> /dev/null; echo $$?), $(shell echo 1))
export CXX = clang++
export LINK = clang++
else
export CXX = g++
export LINK = g++
endif

export CFLAGS = -Wall -W -Wabi -Weffc++ -Wformat -Wshadow -Wsequence-point -Wuninitialized -Wfloat-equal -O3 -c
export CXXFLAGS = -Wall -W -Wabi -Weffc++ -Wformat -Wshadow -Wsequence-point -Wuninitialized -Wfloat-equal -O3 -c
export LFLAGS = -Wall -W -Wabi -Weffc++ -Wformat -Wshadow -Wsequence-point -Wuninitialized -Wfloat-equal -s -O3

# Shell commands
export MV = mv -f
export CP = cp -f -r
export UPDATE = $(CP) -u
export RM = rm -f
export MKDIR = mkdir -p

.PHONY: all

all: oichecker

oichecker: checker.o gen.o oichecker.o
	$(CXX) $^ $(LDFLAGS) -o $@

checker.o: checker.cpp checker.hpp
	$(CXX) $< -c $(CXXFLAGS) -o $@

gen.o: gen.cpp gen.hpp
	$(CXX) $< -c $(CXXFLAGS) -o $@

oichecker.o: oichecker.cpp checker.hpp gen.hpp
	$(CXX) $< -c $(CXXFLAGS) -o $@