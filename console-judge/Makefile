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

export CFLAGS = -Wall -Wextra -Wabi -Weffc++ -Wshadow -Wfloat-equal -Wno-unused-result -O3 -c
export CXXFLAGS = -Wall -Wextra -Wabi -Weffc++ -Wshadow -Wfloat-equal -Wno-unused-result -O3 -c
export LFLAGS = -Wall -Wextra -Wabi -Weffc++ -Wshadow -Wfloat-equal -Wno-unused-result -s -O3

# Shell commands
export MV = mv -f
export CP = cp -rf
export UPDATE = $(CP) -u
export RM = rm -f
export MKDIR = mkdir -p

.PHONY: all install clean

all: judge

judge: main.cpp
	$(LINK) $^ $(LFLAGS) -o $@

install:
	$(CP) judge /usr/local/bin/

clean:
	$(RM) *.o