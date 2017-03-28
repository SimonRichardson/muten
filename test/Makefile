ifndef RM_INCLUDE_DIR
	RM_INCLUDE_DIR=../deps
endif

ifndef RMUTIL_LIBDIR
	RMUTIL_LIBDIR=$(RM_INCLUDE_DIR)/rmutil
endif

ifndef SRC_DIR
	SRC_DIR=../src
endif

CFLAGS = -Wall -g -fPIC -lc -O2 -std=gnu99
CC=gcc

OBJS=$(SRC_DIR)/muten.o $(RMUTIL_LIBDIR)/*.o

muten_test: muten_test.c $(OBJS)
	$(CC) $@.c $(OBJS) -o $@

clean:
	rm -rf muten_test *.xo *.so *.o
