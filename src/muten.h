#ifndef __MUTEN_H__
#define __MUTEN_H__

#define MUTEN_OK 0
#define MUTEN_ERR 1

#define MUTEN_INSERT_SUFFIX "+"
#define MUTEN_DELETE_SUFFIX "-"

#define MUTEN_SEPARATOR = ","

int Muten_Command(char *insertion, char *deletion);

#endif
