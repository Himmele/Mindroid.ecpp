# GCC, the GNU Compiler Collection
CC := gcc
CXX := g++
LD := g++
AS := gas
AR := ar
RM := rm
INCLUDES := -I../src -I../src/posix
CFLAGS := -c -g -O0
LDFLAGS := -lmindroid -lpthread -lrt -L../lib/posix

SRCS = Main.cpp \
	ServiceManager.cpp \
	HelloWorld.cpp

OBJS = $(SRCS:.cpp=.o)

%.o: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

main: $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	$(RM) -f *.o main

