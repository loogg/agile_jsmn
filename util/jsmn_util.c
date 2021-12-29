/**
 * @file    jsmn_util.c
 * @brief   Agile Jsmn 软件包源文件
 * @author  马龙伟 (2544047213@qq.com)
 * @version 1.0.1
 * @date    2021-12-29
 *
 @verbatim
    使用：
    - jsmn_init 初始化 jsmn 解析器
    - 使用 jsmn_parse 将 json 数据解析到 jsmntok_t 数组中
    - JSMN_ItemInit 初始化根节点
    - 调用相应 API 获取自己想要的 json 对象
    - 值返回的是字符串
      如果值是整数或浮点数，需要自己使用 atoi 或 atof 转换
      如果需要判断值是不是 null，需要对字符串进行 strstr / strcmp 判断

 @endverbatim
 *
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 Ma Longwei.
 * All rights reserved.</center></h2>
 *
 */

#include "jsmn_util.h"
#include <string.h>

/** @defgroup AGILE_JSMN_Private_Functions Agile Jsmn Private Functions
 * @{
 */

/**
 * @brief   string 类型 token 字符串比较
 * @param   js json 字符串
 * @param   t token 对象
 * @param   s 要比较的字符串
 * @return  1:相同; 0:不同
 */
static int json_token_streq(const char *js, jsmntok_t *t, const char *const s)
{
    return (strncmp(js + t->start, s, t->end - t->start) == 0 && strlen(s) == (size_t)(t->end - t->start));
}

/**
 * @}
 */

/** @defgroup AGILE_JSMN_Exported_Functions Agile Jsmn Exported Functions
 * @{
 */

/**
 * @brief   初始化 Agile Jsmn 对象
 * @param   item Agile Jsmn 对象指针
 * @param   t 对应的 token
 * @param   index 对应的 token 索引
 * @param   tokens_len 数组剩余 token 数目
 */
void JSMN_ItemInit(jsmn_item_t *item, jsmntok_t *t, int index, int tokens_len)
{
    item->t = t;
    item->index = index;
    item->left_num = tokens_len - index - 1;
}

/**
 * @brief   根据 key 从 object 中获取 Agile Jsmn 对象
 * @param   js json 字符串
 * @param   object object 对象
 * @param   string key 字符串
 * @param   item 存放的 Agile Jsmn 对象指针
 * @return  0:成功; !=0:异常
 */
int JSMN_GetObjectItem(const char *js, jsmn_item_t *object, const char *const string, jsmn_item_t *item)
{
    if ((js == NULL) || (object == NULL) || (string == NULL) || (item == NULL))
        return -1;

    jsmn_item_t ob_item;

    if (object->t->type == JSMN_OBJECT) {
        ob_item.t = object->t;
        ob_item.index = object->index;
        ob_item.left_num = object->left_num;
    } else {
        if ((object->t->type != JSMN_STRING) || (object->t->size != 1) || (object->left_num) <= 0)
            return -1;

        jsmntok_t *t = object->t + 1;
        if ((t->type != JSMN_OBJECT) || (t->parent != object->index))
            return -1;

        ob_item.t = object->t + 1;
        ob_item.index = object->index + 1;
        ob_item.left_num = object->left_num - 1;
    }

    if (ob_item.left_num <= 0)
        return -1;

    for (int i = 1; i <= ob_item.left_num; i++) {
        jsmntok_t *t = ob_item.t + i;
        if ((t->type == JSMN_STRING) && (t->parent == ob_item.index)) {
            if (json_token_streq(js, t, string)) {
                item->t = t;
                item->index = ob_item.index + i;
                item->left_num = ob_item.left_num - i;

                return 0;
            }
        }
    }

    return -1;
}

/**
 * @brief   获取 Agile Jsmn 对象的字符串
 * @note    适用 "key":$value 形式。
 *          - 该 API 会修改 'js' json 字符串，请确保 'js' 参数指向的内存可更改，否则请使用 'JSMN_GetStringBuffered'
 *          - 如果 Agile Jsmn 对象指向的是 key，返回的就是 key 字符串
 *          - 如果 Agile Jsmn 对象指向的是 value，返回的就是 value 值。返回的是字符串，需要自行处理判断
 * @par     $value 示例
 * @code
 *          "key":"123" 形式返回 123
 *          "key":123 形式返回 123
 *          "key":null 形式返回 null
 * @endcode
 * @param   js json 字符串
 * @param   item Agile Jsmn 对象
 * @return  !=NULL:字符串; =NULL:异常
 */
char *JSMN_GetString(char *js, jsmn_item_t *item)
{
    if ((js == NULL) || (item == NULL))
        return NULL;

    if ((item->t->type != JSMN_STRING) && (item->t->type != JSMN_PRIMITIVE))
        return NULL;

    js[item->t->end] = '\0';
    return (js + item->t->start);
}

/**
 * @brief   获取 Agile Jsmn 对象的字符串并存入 buf
 * @note    该 API 功能与 'JSMN_GetString' 相同。如果字符串长度比 'bufsz - 1' 大，则会自动丢弃多余的。
 * @param   js json 字符串
 * @param   item Agile Jsmn 对象
 * @param   buf 存储内存
 * @param   bufsz 存储内存大小
 * @return  >=0:字符串长度; 其他:异常
 */
int JSMN_GetStringBuffered(const char *js, jsmn_item_t *item, char *buf, int bufsz)
{
    memset(buf, 0, bufsz);

    if ((js == NULL) || (item == NULL) || (buf == NULL) || (bufsz <= 0))
        return -1;

    if ((item->t->type != JSMN_STRING) && (item->t->type != JSMN_PRIMITIVE))
        return -1;

    int len = item->t->end - item->t->start;
    if (len >= bufsz)
        len = bufsz - 1;

    memcpy(buf, js + item->t->start, len);

    return len;
}

/**
 * @brief   获取 Agile Jsmn 对象的值字符串
 * @note    适用 "key":$value 形式，Agile Jsmn 对象指向的是 key，获取的是 value 值
 * @see     value 值参见 JSMN_GetString
 * @param   js json 字符串
 * @param   item Agile Jsmn 对象
 * @return  !=NULL:值字符串; =NULL:异常
 */
char *JSMN_GetValueString(char *js, jsmn_item_t *item)
{
    if ((js == NULL) || (item == NULL))
        return NULL;

    if ((item->t->type != JSMN_STRING) || (item->t->size != 1) || (item->left_num <= 0))
        return NULL;

    jsmn_item_t val_item;
    val_item.t = item->t + 1;
    val_item.index = item->index + 1;
    val_item.left_num = item->left_num - 1;

    return JSMN_GetString(js, &val_item);
}

/**
 * @brief   获取 Agile Jsmn 对象的值字符串并存入 buf
 * @note    该 API 功能与 'JSMN_GetValueString' 相同。如果字符串长度比 'bufsz - 1' 大，则会自动丢弃多余的。
 * @param   js json 字符串
 * @param   item Agile Jsmn 对象
 * @param   buf 存储内存
 * @param   bufsz 存储内存大小
 * @return  >=0:字符串长度; 其他:异常
 */
int JSMN_GetValueStringBuffered(const char *js, jsmn_item_t *item, char *buf, int bufsz)
{
    if ((js == NULL) || (item == NULL) || (buf == NULL) || (bufsz <= 0))
        return -1;

    if ((item->t->type != JSMN_STRING) || (item->t->size != 1) || (item->left_num <= 0))
        return -1;

    jsmn_item_t val_item;
    val_item.t = item->t + 1;
    val_item.index = item->index + 1;
    val_item.left_num = item->left_num - 1;

    return JSMN_GetStringBuffered(js, &val_item, buf, bufsz);
}

/**
 * @brief   获取 json 数组成员数目
 * @param   array Agile Jsmn 数组类型对象
 * @return  >=0:数组成员数目; 其他:异常
 */
int JSMN_GetArraySize(jsmn_item_t *array)
{
    if (array == NULL)
        return -1;

    if (array->t->type == JSMN_ARRAY)
        return array->t->size;

    if ((array->t->type != JSMN_STRING) || (array->t->size != 1) || (array->left_num <= 0))
        return -1;

    jsmntok_t *t = array->t + 1;
    if ((t->type != JSMN_ARRAY) || (t->parent != array->index))
        return -1;

    return t->size;
}

/**
 * @brief   根据索引获取 json 数组中的成员对象
 * @param   array Agile Jsmn 数组类型对象
 * @param   index 索引
 * @param   item 存放的 Agile Jsmn 对象指针
 * @return  =0:成功; !=0:异常
 */
int JSMN_GetArrayItem(jsmn_item_t *array, int index, jsmn_item_t *item)
{
    if ((array == NULL) || (index < 0) || (item == NULL))
        return -1;

    if (index >= JSMN_GetArraySize(array))
        return -1;

    jsmn_item_t array_tok;

    if (array->t->type == JSMN_ARRAY) {
        array_tok.t = array->t;
        array_tok.index = array->index;
        array_tok.left_num = array->left_num;
    } else {
        array_tok.t = array->t + 1;
        array_tok.index = array->index + 1;
        array_tok.left_num = array->left_num - 1;
    }

    for (int i = 1; i <= array_tok.left_num; i++) {
        jsmntok_t *t = array_tok.t + i;
        if (t->parent == array_tok.index) {
            if (index > 0) {
                index--;
                continue;
            }
            item->t = t;
            item->index = array_tok.index + i;
            item->left_num = array_tok.left_num - i;

            return 0;
        }
    }

    return -1;
}

/**
 * @}
 */
