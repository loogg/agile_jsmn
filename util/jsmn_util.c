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
    if((js == NULL) || (object == NULL) || (string == NULL) || (item == NULL))
        return -1;

    jsmn_item_t ob_item;

    if(object->t->type == JSMN_OBJECT)
    {
        ob_item.t = object->t;
        ob_item.index = object->index;
        ob_item.left_num = object->left_num;
    }
    else
    {
        if((object->t->type != JSMN_STRING) || (object->t->size != 1) || (object->left_num) <= 0)
            return -1;

        jsmntok_t *t = object->t + 1;
        if((t->type != JSMN_OBJECT) || (t->parent != object->index))
            return -1;
        
        ob_item.t = object->t + 1;
        ob_item.index = object->index + 1;
        ob_item.left_num = object->left_num - 1;
    }

    if(ob_item.left_num <= 0)
        return -1;

    for(int i = 1; i <= ob_item.left_num; i++)
    {
        jsmntok_t *t = ob_item.t + i;
        if((t->type == JSMN_STRING) && (t->parent == ob_item.index))
        {
            if(json_token_streq(js, t, string))
            {
                item->t = t;
                item->index = ob_item.index + i;
                item->left_num = ob_item.left_num - i;

                return 0;
            }
        }
    }

    return -1;
}

char *JSMN_GetString(char *js, jsmn_item_t *item)
{
    if((js == NULL) || (item == NULL))
        return NULL;
    
    if((item->t->type != JSMN_STRING) && (item->t->type != JSMN_PRIMITIVE))
        return NULL;
    
    js[item->t->end] = '\0';
    return (js + item->t->start);
}

int JSMN_GetStringBuffered(const char *js, jsmn_item_t *item, char *buf, int bufsz)
{
    memset(buf, 0, bufsz);

    if((js == NULL) || (item == NULL) || (buf == NULL) || (bufsz <= 0))
        return -1;
    
    if((item->t->type != JSMN_STRING) && (item->t->type != JSMN_PRIMITIVE))
        return -1;
    
    int len = item->t->end - item->t->start;
    if(len >= bufsz)
        len = bufsz - 1;
    
    memcpy(buf, js + item->t->start, len);
    return len;
}

char *JSMN_GetValueString(char *js, jsmn_item_t *item)
{
    if((js == NULL) || (item == NULL))
        return NULL;
    
    if((item->t->type != JSMN_STRING) || (item->t->size != 1) || (item->left_num <= 0))
        return NULL;
    
    jsmn_item_t val_item;
    val_item.t = item->t + 1;
    val_item.index = item->index + 1;
    val_item.left_num = item->left_num - 1;
    
    return JSMN_GetString(js, &val_item);
}

int JSMN_GetValueStringBuffered(const char *js, jsmn_item_t *item, char *buf, int bufsz)
{
    if((js == NULL) || (item == NULL) || (buf == NULL) || (bufsz <= 0))
        return -1;
    
    if((item->t->type != JSMN_STRING) || (item->t->size != 1) || (item->left_num <= 0))
        return -1;
    
    jsmn_item_t val_item;
    val_item.t = item->t + 1;
    val_item.index = item->index + 1;
    val_item.left_num = item->left_num - 1;

    return JSMN_GetStringBuffered(js, &val_item, buf, bufsz);
}

int JSMN_GetArraySize(jsmn_item_t *array)
{
    if(array == NULL)
        return -1;
    
    if(array->t->type == JSMN_ARRAY)
        return array->t->size;
    
    if((array->t->type != JSMN_STRING) || (array->t->size != 1) || (array->left_num <= 0))
        return -1;
    
    jsmntok_t *t = array->t + 1;
    if((t->type != JSMN_ARRAY) || (t->parent != array->index))
        return -1;
    
    return t->size;
}

int JSMN_GetArrayItem(jsmn_item_t *array, int index, jsmn_item_t *item)
{
    if((array == NULL) || (index < 0) || (item == NULL))
        return -1;

    if(index >= JSMN_GetArraySize(array))
        return -1;
    
    jsmn_item_t array_tok;

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

    for (int i = 1; i <= array_tok.left_num; i++)
    {
        jsmntok_t *t = array_tok.t + i;
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
