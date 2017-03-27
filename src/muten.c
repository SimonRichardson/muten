/*
* muten - is a implementation of the roshi set using the redis module SDK rather
* than using the Lua vm.
*/

#include <stdio.h>
#include <string.h>
#include "../deps/redismodule.h"
#include "../deps/logging/logging.h"
#include "../deps/rmutil/sds.h"
#include "muten.h"

#define RM_MODULE_NAME "muten"
#define RLMODULE_VERSION "1.0.0"
#define RLMODULE_PROTO "1.0"
#define RLMODULE_DESC "Reasonable implmentation of the roshi set."

#define MUTEN_SIGNATURE "MUTEN:1.0:"

int Muten_Validate(char *str, long long score, const char *txn) {
  return MUTEN_OK;
}

/* MUTEN.INSERT key field score txn data
*/
int MutenInsertCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
  if (argc != 5) {
    return RedisModule_WrongArity(ctx);
  }
  RedisModule_AutoMemory(ctx);

  // Define the keys for both types of sets
  size_t skeyLen;
  const char *skey = RedisModule_StringPtrLen(argv[1], &skeyLen);

  sds ikey = sdsempty();
  ikey = sdscat(ikey, skey);
  ikey = sdscat(ikey, MUTEN_INSERT_SUFFIX);

  sds dkey = sdsempty();
  dkey = sdscat(dkey, skey);
  dkey = sdscat(dkey, MUTEN_DELETE_SUFFIX);

  RedisModuleString *isKey = RedisModule_CreateString(ctx, (const char *)ikey, sdslen(ikey));
  RedisModuleString *dsKey = RedisModule_CreateString(ctx, (const char *)dkey, sdslen(dkey));

  sdsfree(ikey);
  sdsfree(dkey);

  // Make sure that the value is a valid hash.
  RedisModuleKey *insertionKey =
      RedisModule_OpenKey(ctx, isKey, REDISMODULE_READ | REDISMODULE_WRITE);
  if ((RedisModule_KeyType(insertionKey) != REDISMODULE_KEYTYPE_EMPTY) &&
      (RedisModule_KeyType(insertionKey) != REDISMODULE_KEYTYPE_HASH)) {
    RedisModule_ReplyWithError(ctx, REDISMODULE_ERRORMSG_WRONGTYPE);
    return REDISMODULE_ERR;
  }

  RedisModuleKey *deletionKey =
      RedisModule_OpenKey(ctx, dsKey, REDISMODULE_READ | REDISMODULE_WRITE);
  if ((RedisModule_KeyType(deletionKey) != REDISMODULE_KEYTYPE_EMPTY) &&
      (RedisModule_KeyType(deletionKey) != REDISMODULE_KEYTYPE_HASH)) {
    RedisModule_ReplyWithError(ctx, REDISMODULE_ERRORMSG_WRONGTYPE);
    return REDISMODULE_ERR;
  }

  // Get the values.
  RedisModuleString *field = argv[2];

  long long score;
  if ((RedisModule_StringToLongLong(argv[2], &score) != REDISMODULE_OK) || (score < 1)) {
    RedisModule_ReplyWithError(ctx, "ERR invalid score");
    return REDISMODULE_ERR;
  }

  size_t txnLen;
  const char *txn = RedisModule_StringPtrLen(argv[4], &txnLen);

  size_t dataLen;
  const char *data = RedisModule_StringPtrLen(argv[5], &dataLen);

  int err;

  // Get the value out of the hash for insertions.
  RedisModuleString *insertion;
  err = RedisModule_HashGet(insertionKey, REDISMODULE_HASH_NONE, field, &insertion, NULL);
  if (err == REDISMODULE_ERR) {
    return REDISMODULE_ERR;
  }
  if (insertion) {
    err = Muten_Validate((char *)insertion, score, txn);
    if (err != MUTEN_OK) {
      LG_DEBUG("Insertion value was invalid!");
      RedisModule_ReplyWithLongLong(ctx, -1);
      return REDISMODULE_OK;
    }
  }

  // Get the value out of the hash for deletions.
  RedisModuleString *deletion;
  err = RedisModule_HashGet(deletionKey, REDISMODULE_HASH_NONE, field, &deletion, NULL);
  if (err == REDISMODULE_ERR) {
    return REDISMODULE_ERR;
  }
  if (deletion) {
    err = Muten_Validate((char *)deletion, score, txn);
    if (err != MUTEN_OK) {
      LG_DEBUG("Deletion value was invalid!");
      RedisModule_ReplyWithLongLong(ctx, -1);
      return REDISMODULE_OK;
    }
  }

  // We don't actually care about the result of this!
  RedisModuleCallReply *res = RedisModule_Call(ctx, "hdel", (char *)dsKey, field);
  if (RedisModule_CallReplyType(res) == REDISMODULE_REPLY_INTEGER && RedisModule_CallReplyInteger(res) != 0) {
    LG_DEBUG("Removal of hash field.");
  } else if (RedisModule_CallReplyType(res) == REDISMODULE_REPLY_ERROR) {
    LG_WARN("Attempting to remove hash key failed.");
    return REDISMODULE_ERR;
  }

  // Now the actual setting.
  int updated = RedisModule_HashSet(insertionKey, REDISMODULE_HASH_NONE, field, data, NULL);
  RedisModule_ReplyWithLongLong(ctx, (long long)updated);

  return REDISMODULE_OK;
}

/* MUTEN.DELETE key field score txn data
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
