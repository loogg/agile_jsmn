/**
 * @file    jsmn_util.h
 * @brief   Agile Jsmn 软件包头文件
 * @author  马龙伟 (2544047213@qq.com)
 * @version 1.0.1
 * @date    2021-12-29
 *
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 Ma Longwei.
 * All rights reserved.</center></h2>
 *
 */

#ifndef __JSMN_UTIL_H
#define __JSMN_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "jsmn.h"

/** @defgroup AGILE_JSMN_Exported_Types Agile Jsmn Exported Types
 * @{
 */

/**
 * @brief   Agile Jsmn 结构体
 */
typedef struct
{
    jsmntok_t *t; /**< 对应 jsmn token 对象 */
    int index;    /**< 对应 jsmn token 对象在 token 数组中的索引 */
    int left_num; /**< token 数组中剩余 token 数目 */
} jsmn_item_t;

/**
 * @}
 */

/** @addtogroup AGILE_JSMN_Exported_Functions
 * @{
 */
void JSMN_ItemInit(jsmn_item_t *item, jsmntok_t *t, int index, int tokens_len);
int JSMN_GetObjectItem(const char *js, jsmn_item_t *object, const char *const string, jsmn_item_t *item);
char *JSMN_GetString(char *js, jsmn_item_t *item);
int JSMN_GetStringBuffered(const char *js, jsmn_item_t *item, char *buf, int bufsz);
char *JSMN_GetValueString(char *js, jsmn_item_t *item);
int JSMN_GetValueStringBuffered(const char *js, jsmn_item_t *item, char *buf, int bufsz);
int JSMN_GetArraySize(jsmn_item_t *array);
int JSMN_GetArrayItem(jsmn_item_t *array, int index, jsmn_item_t *item);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __JSMN_UTIL_H */
