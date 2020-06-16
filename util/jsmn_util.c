#include "jsmn_util.h"
#include <string.h>

static int json_token_streq(const char *js, jsmntok_t *t, const char * const s)
{
    return (strncmp(js + t->start, s, t->end - t->start) == 0
            && strlen(s) == (size_t) (t->end - t->start));
}

void JSMN_ItemInit(jsmn_item_t *item, jsmntok_t *t, int index, int tokens_len)
{
    item->t = t;
    item->index = index;
    item->left_num = tokens_len - index - 1;
}

int JSMN_GetObjectItem(const char *js, jsmn_item_t *object, const char * const string, jsmn_item_t *item)
{
    int i;
    jsmntok_t *t = NULL;
    if((js == NULL) || (object == NULL) || (string == NULL) || (item == NULL))
        return -1;
    
    if(object->t->type != JSMN_OBJECT)
        return -1;
    
    if(object->left_num <= 0)
        return -1;

    for(i = 1; i <= object->left_num; i++)
    {
        t = object->t + i;
        if((t->type == JSMN_STRING) && (t->parent == object->index))
        {
            if(json_token_streq(js, t, string))
            {
                item->t = t;
                item->index = object->index + i;
                item->left_num = object->left_num - i;

                return 0;
            }
        }
    }

    return -1;
}

char *JSMN_GetValueString(char *js, jsmn_item_t *item)
{
    jsmntok_t *t = NULL;
    if((js == NULL) || (item == NULL))
        return NULL;
    
    if((item->t->type != JSMN_STRING) || (item->t->size != 1) || (item->left_num <= 0))
        return NULL;
    
    t = item->t + 1;

    if((t->type != JSMN_STRING) || (t->size != 0) || (t->parent != item->index))
        return NULL;
    
    js[t->end] = '\0';
    return js + t->start;
}

int JSMN_GetValueStringBuffered(const char *js, jsmn_item_t *item, char *buf, int bufsz)
{
    memset(buf, 0, bufsz);
    jsmntok_t *t = NULL;
    int len;

    if((js == NULL) || (item == NULL) || (buf == NULL) || (bufsz <= 0))
        return -1;
    
    if((item->t->type != JSMN_STRING) || (item->t->size != 1) || (item->left_num <= 0))
        return -1;
    
    t = item->t + 1;

    if((t->type != JSMN_STRING) || (t->size != 0) || (t->parent != item->index))
        return -1;
    
    len = t->end - t->start;

    if(bufsz <= len)
        return -1;
    
    memcpy(buf, js + t->start, len);
    return 0;
}

int JSMN_GetArraySize(jsmn_item_t *array)
{
    jsmntok_t *t = NULL;

    if(array == NULL)
        return -1;
    
    if(array->t->type == JSMN_ARRAY)
        return array->t->size;
    
    if((array->t->type != JSMN_STRING) || (array->t->size != 1) || (array->left_num <= 0))
        return -1;
    
    t = array->t + 1;

    if((t->type != JSMN_ARRAY) || (t->parent != array->index))
        return -1;
    
    return t->size;
}

int JSMN_GetArrayItem(jsmn_item_t *array, int index, jsmn_item_t *item)
{
    jsmn_item_t array_tok;
    int i;
    jsmntok_t *t = NULL;
    if((array == NULL) || (index < 0) || (item == NULL))
        return -1;

    if(index >= JSMN_GetArraySize(array))
        return -1;
    
    if(array->t->type == JSMN_ARRAY)
    {
        array_tok.t = array->t;
        array_tok.index = array->index;
        array_tok.left_num = array->left_num;
    }
    else
    {
        array_tok.t = array->t + 1;
        array_tok.index = array->index + 1;
        array_tok.left_num = array->left_num - 1;
    }

    for (i = 1; i <= array_tok.left_num; i++)
    {
        t = array_tok.t + i;
        if(t->parent == array_tok.index)
        {
            if(index > 0)
            {
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
