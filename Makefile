CC := gcc
CXX := g++
LD := g++
AS := gas
AR := ar
RM := rm
INCLUDES := -I.
CFLAGS := -c -g -O0
LDFLAGS :=
OUT_DIR := out/posix

SRCS = mindroid/app/Process.cpp \
	mindroid/os/Clock.cpp \
	mindroid/os/CondVar.cpp \
	mindroid/os/Handler.cpp \
	mindroid/os/Lock.cpp \
	mindroid/os/Looper.cpp \
	mindroid/os/Message.cpp \
	mindroid/os/MessageQueue.cpp \
	mindroid/os/RunnableQueue.cpp \
	mindroid/os/Thread.cpp \
	mindroid/util/Assert.cpp \
	mindroid/util/Log.cpp \
	mindroid/util/Logger.cpp

OBJS = $(SRCS:.cpp=.o)
OUT_OBJS = $(addprefix $(OUT_DIR)/,$(OBJS))

Mindroid.ecpp = $(OUT_DIR)/libmindroid.a

.PHONY: depend clean

all: $(Mindroid.ecpp)

$(Mindroid.ecpp): $(OUT_OBJS) 
	$(AR) -r $@ $^

$(OUT_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) -rf $(OUT_DIR)

depend: $(SRCS)
	makedepend $(INCLUDES) $^
