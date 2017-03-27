#set environment variable RM_INCLUDE_DIR to the location of redismodule.h
ifndef RM_INCLUDE_DIR
	RM_INCLUDE_DIR=./deps
endif

ifndef RMUTIL_LIBDIR
	RMUTIL_LIBDIR=$(RM_INCLUDE_DIR)/rmutil
endif

ifndef RMLOGGING_LIBDIR
	RMLOGGING_LIBDIR=$(RM_INCLUDE_DIR)/logging
endif

ifndef SRC_DIR
	SRC_DIR=./src
endif

ifndef TEST_DIR
	TEST_DIR=./test
endif

.PHONY: default
default:
	@echo "Explicit target required"

all: muten.so

.PHONY: test
test: muten.so
	$(MAKE) -C $(TEST_DIR) ${RUN}
	@(sh -c ./test/muten_test)

muten.so:
	$(MAKE) -C $(SRC_DIR)
	cp $(SRC_DIR)/muten.so .

clean: FORCE
	rm -rf *.xo *.so *.o
	rm -rf $(SRC_DIR)/*.xo $(SRC_DIR)/*.so $(SRC_DIR)/*.o
	rm -rf $(TEST_DIR)/*.xo $(TEST_DIR)/*.so $(TEST_DIR)/*.o
	rm -rf $(RMUTIL_LIBDIR)/*.so $(RMUTIL_LIBDIR)/*.o $(RMUTIL_LIBDIR)/*.a
	rm -rf $(RMLOGGING_LIBDIR)/*.so $(RMLOGGING_LIBDIR)/*.o $(RMLOGGING_LIBDIR)/*.a

FORCE:
