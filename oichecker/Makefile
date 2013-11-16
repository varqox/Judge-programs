CXX = clang++
CXXFLAGS = -O3 -static -m32 -Wall
LDFLAGS = -s $(CXXFLAGS)

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