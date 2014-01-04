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

export CFLAGS = -Wall -O3 -c
export CXXFLAGS = -Wall -O3 -c
export LFLAGS = -Wall -s -O3

# Shell commands
export MV = mv -f
export CP = cp -f -r
export UPDATE = $(CP) -u
export RM = rm -f
export MKDIR = mkdir -p

.PHONY: all clean

all: judge

judge: main.cpp
	$(LINK) $^ $(LFLAGS) -o $@

clean:
	$(RM) *.o