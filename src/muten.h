#ifndef __MUTEN_H__
#define __MUTEN_H__

#include "../deps/redismodule.h"

#define MUTEN_OK 0
#define MUTEN_ERR 1

#define MUTEN_INSERT_SUFFIX "+"
#define MUTEN_DELETE_SUFFIX "-"

#define MUTEN_SEPARATOR = ","

int Muten_Validate(char *str, long long score, const char *txn);

#endif
