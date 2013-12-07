CXX = clang++
LINK = clang++
CXXFLAGS = -O3
LFLAGS = -s $(CXXFLAGS)
CP = cp
RM = rm -f

.PHONY: all clean

all: judge

judge: main.cpp
	$(LINK) $^ $(LFLAGS) -o $@

clean:
	$(RM) *.o