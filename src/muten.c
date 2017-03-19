/*
* muten - is a implementation of the roshi set using the redis module SDK rather
* than using the Lua vm.
*/

#include <stdio.h>
#include <string.h>
#include "../deps/redismodule.h"
#include "../deps/logging/logging.h"
#include "muten.h"

#define RM_MODULE_NAME "muten"
#define RLMODULE_VERSION "1.0.0"
#define RLMODULE_PROTO "1.0"
#define RLMODULE_DESC "Reasonable implmentation of the roshi set."

#define MUTEN_SIGNATURE "MUTEN:1.0:"

int Muten_Command(char *insertion, char *deletion) {
  LG_DEBUG("Command");
  return MUTEN_OK;
}

/* MUTEN.INSERT key field score txn expiry data
*/
int MutenInsertCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
  if (argc != 6) {
    return RedisModule_WrongArity(ctx);
  }
  RedisModule_AutoMemory(ctx);

  // Make sure that the value is a valid hash.
  RedisModuleKey *key =
      RedisModule_OpenKey(ctx, argv[1], REDISMODULE_READ | REDISMODULE_WRITE);
  if ((RedisModule_KeyType(key) != REDISMODULE_KEYTYPE_EMPTY) &&
      (RedisModule_KeyType(key) != REDISMODULE_KEYTYPE_HASH)) {
    RedisModule_ReplyWithError(ctx, REDISMODULE_ERRORMSG_WRONGTYPE);
    return REDISMODULE_ERR;
  }

  int err = Muten_Command(MUTEN_INSERT_SUFFIX, MUTEN_DELETE_SUFFIX);
  if (err != MUTEN_OK) {
    return REDISMODULE_ERR;
  }

  return REDISMODULE_OK;
}

/* MUTEN.DELETE key field score txn expiry data
*/
int MutenDeleteCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
  if (argc != 6) {
    return RedisModule_WrongArity(ctx);
  }
  RedisModule_AutoMemory(ctx);

    // Make sure that the value is a valid hash.
  RedisModuleKey *key =
      RedisModule_OpenKey(ctx, argv[1], REDISMODULE_READ | REDISMODULE_WRITE);
  if ((RedisModule_KeyType(key) != REDISMODULE_KEYTYPE_EMPTY) &&
      (RedisModule_KeyType(key) != REDISMODULE_KEYTYPE_HASH)) {
    RedisModule_ReplyWithError(ctx, REDISMODULE_ERRORMSG_WRONGTYPE);
    return REDISMODULE_ERR;
  }

  int err = Muten_Command(MUTEN_DELETE_SUFFIX, MUTEN_INSERT_SUFFIX);
  if (err != MUTEN_OK) {
    return REDISMODULE_ERR;
  }

  return REDISMODULE_OK;
}

/* MUTEN.DEBUG key
*/
int MutenDebugCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
  if (argc != 2) {
    return RedisModule_WrongArity(ctx);
  }
  RedisModule_AutoMemory(ctx);

  RedisModuleKey *key = RedisModule_OpenKey(ctx, argv[1], REDISMODULE_READ);

  /* Verify that the key is empty or a string. */
  if (RedisModule_KeyType(key) != REDISMODULE_KEYTYPE_EMPTY) {
    RedisModule_ReplyWithNull(ctx);
    return REDISMODULE_OK;
  }

  RedisModule_ReplyWithSimpleString(ctx, "PASS");
  return REDISMODULE_OK;
}

int RedisModule_OnLoad(RedisModuleCtx *ctx) {
  if (RedisModule_Init(ctx, RM_MODULE_NAME, 1, REDISMODULE_APIVER_1) ==
      REDISMODULE_ERR) {
    return REDISMODULE_ERR;
  }

  if (RedisModule_CreateCommand(ctx, "muten.insert", MutenInsertCommand, "write deny-oom",
                                1, 2, 1) == REDISMODULE_ERR) {
    return REDISMODULE_ERR;
  }

  if (RedisModule_CreateCommand(ctx, "muten.delete", MutenDeleteCommand, "write deny-oom",
                                1, 2, 1) == REDISMODULE_ERR) {
    return REDISMODULE_ERR;
  }

  if (RedisModule_CreateCommand(ctx, "muten.debug", MutenDebugCommand, "readonly",
                                1, 1, 1) == REDISMODULE_ERR) {
    return REDISMODULE_ERR;
  }

  LOGGING_INIT(L_DEBUG);
  LG_DEBUG("module loaded ok.");

  return REDISMODULE_OK;
}
