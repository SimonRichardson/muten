#set environment variable RM_INCLUDE_DIR to the location of redismodule.h
ifndef RM_INCLUDE_DIR
	RM_INCLUDE_DIR=../deps
endif

ifndef RMUTIL_LIBDIR
	RMUTIL_LIBDIR=$(RM_INCLUDE_DIR)/rmutil
endif

# find the OS
uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')

# Compile flags for linux / osx
ifeq ($(uname_S),Linux)
	SHOBJ_CFLAGS ?=  -fno-common -g -ggdb
	SHOBJ_LDFLAGS ?= -shared -Bsymbolic
else
	SHOBJ_CFLAGS ?= -dynamic -fno-common -g -ggdb
	SHOBJ_LDFLAGS ?= -bundle -undefined dynamic_lookup
endif

CFLAGS = -I$(RM_INCLUDE_DIR) -Wall -g -fPIC -lc -lm -O2 -std=gnu99
CC=gcc

all: rmutil muten.so

rmutil: FORCE
	$(MAKE) -C $(RMUTIL_LIBDIR)

muten.so: muten.o
	$(LD) -o $@ muten.o $(SHOBJ_LDFLAGS) $(LIBS) -L$(RMUTIL_LIBDIR) -lrmutil -lc

clean:
	rm -rf *.xo *.so *.o

FORCE:
