#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsmn.h"
#include "jsmn_util.h"

#define DBG_ENABLE
#define DBG_COLOR
#define DBG_LEVEL DBG_LOG
#define DBG_SECTION_NAME "example_jsmn"
#include "dbg_log.h"

#define EXAMPLE_FILE_PATH   "example.json"

int main(void)
{
    system("cls");
    FILE *fp = fopen(EXAMPLE_FILE_PATH, "rb");
    if(!fp)
    {
        LOG_E("open file:%s error", EXAMPLE_FILE_PATH);
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    int nFileLen = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *js = (char *)malloc(nFileLen + 1);
    fread(js, nFileLen, 1, fp);
    fclose(fp);
    js[nFileLen] = 0;

    int result = 0;
    jsmn_parser parser;
    jsmn_init(&parser);
    jsmntok_t t[100] = {0};
    int len = jsmn_parse(&parser, js, strlen(js), t, sizeof(t) / sizeof(t[0]));
    if(len <= 0)
    {
        LOG_E("parse error.");
        result = -1;
        goto _exit;
    }

    jsmn_item_t root;
    JSMN_ItemInit(&root, t, 0, len);

    int root_array_size = JSMN_GetArraySize(&root);
    if(root_array_size < 0)
    {
        LOG_E("root array error. (size:%d)", root_array_size);
        result = -1;
        goto _exit;
    }

    for (int i = 0; i < root_array_size; i++)
    {
        jsmn_item_t root_array_item;
        JSMN_GetArrayItem(&root, i, &root_array_item);
        
        jsmn_item_t root_array_item_a;
        if(JSMN_GetObjectItem(js, &root_array_item, "a", &root_array_item_a) != 0)
        {
            result = -1;
            goto _exit;
        }

        LOG_I("get root array index:%d--->\"a\":%s", i, JSMN_GetItemValueStringOnlyMemory(js, &root_array_item_a));

        jsmn_item_t b_array;
        if(JSMN_GetObjectItem(js, &root_array_item, "b", &b_array) != 0)
        {
            result = -1;
            goto _exit;
        }

        int b_array_size = JSMN_GetArraySize(&b_array);
        if(b_array_size < 0)
        {
            result = -1;
            goto _exit;
        }

        for (int j = 0; j < b_array_size; j++)
        {
            jsmn_item_t b_array_item;
            JSMN_GetArrayItem(&b_array, j, &b_array_item);
            
            jsmn_item_t b_array_item_a;
            if(JSMN_GetObjectItem(js, &b_array_item, "a", &b_array_item_a) != 0)
            {
                result = -1;
                goto _exit;
            }

            LOG_I("get root array index:%d--->\"b\" array index:%d--->\"a\":%s", i, j, JSMN_GetItemValueStringOnlyMemory(js, &b_array_item_a));
        }
    }

_exit:
    free(js);
    getchar();
    return result;
}
