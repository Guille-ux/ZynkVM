
CC = gcc
CFLAGS = -Wall -Wextra -fPIC 
AR = ar
ARFLAGS = rcs
RM = rm -f
BUILD = lib


STATIC_LIB = libsysarena.a
SHARED_LIB = libsysarena.so


SRC = src
SOURCE = sysarena.c


OBJECT = $(SOURCE:.c=.o) 

all: $(STATIC_LIB) $(SHARED_LIB)
all: move

move:
	mv sysarena.o lib
	mv libsysarena.a lib
	mv libsysarena.so lib

$(OBJECT): $(SRC)/$(SOURCE)
	$(CC) $(CFLAGS) -c $< -o $@


$(STATIC_LIB): $(OBJECT)
	$(AR) $(ARFLAGS) $@ $^

$(SHARED_LIB): $(OBJECT)
	$(CC) -shared $^ -o $@

clean:
	$(RM) $(BUILD)/$(OBJECT) $(BUILD)/$(STATIC_LIB) $(BUILD)/$(SHARED_LIB)

.PHONY: all clean