#ifndef __JSMN_UTIL_H
#define __JSMN_UTIL_H

#include "jsmn.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
    jsmntok_t *t;
    int index;
    int left_num;
} jsmn_item_t;


void JSMN_ItemInit(jsmn_item_t *item, jsmntok_t *t, int index, int tokens_len);
int JSMN_GetObjectItem(const char *js, jsmn_item_t *object, const char * const string, jsmn_item_t *item);
char *JSMN_GetValueString(char *js, jsmn_item_t *item);
int JSMN_GetValueStringBuffered(const char *js, jsmn_item_t *item, char *buf, int bufsz);
int JSMN_GetArraySize(jsmn_item_t *array);
int JSMN_GetArrayItem(jsmn_item_t *array, int index, jsmn_item_t *item);

#ifdef __cplusplus
}
#endif

#endif
