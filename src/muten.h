/*
* Copyright (C) 2017 Simon Richardson
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as
* published by the Free Software Foundation, either version 3 of the
* License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __MUTEN_H__
#define __MUTEN_H__

#include "../deps/redismodule.h"

#define MUTEN_OK 0
#define MUTEN_ERR 1
#define MUTEN_INVALID_ERR 2

#define MUTEN_INSERT_SUFFIX "+"
#define MUTEN_DELETE_SUFFIX "-"

#define MUTEN_SEPARATOR = ","

#define MUTEN_ERRORMSG_WRONGKEY "WRONGKEY Operation against a key holding the wrong kind of value"

int Muten_Validate(char *str, long long score, const char *txn);

#endif
