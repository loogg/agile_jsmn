#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsmn.h"
#include "jsmn_util.h"

#define DBG_ENABLE
#define DBG_COLOR
#define DBG_LEVEL        DBG_LOG
#define DBG_SECTION_NAME "example_jsmn"
#include "dbg_log.h"

#define EXAMPLE_FILE_PATH "example.json"

int main(void)
{
    system("cls");
    FILE *fp = fopen(EXAMPLE_FILE_PATH, "rb");
    if (!fp) {
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

    do {
        jsmn_parser parser;
        jsmn_init(&parser);
        jsmntok_t tokens[100] = {0};
        int tokens_len = jsmn_parse(&parser, js, strlen(js), tokens, sizeof(tokens) / sizeof(tokens[0]));
        if (tokens_len <= 0)
            break;
        jsmn_item_t root;
        JSMN_ItemInit(&root, tokens, 0, tokens_len);
        int root_array_size = JSMN_GetArraySize(&root);
        if (root_array_size < 0)
            break;
        for (int i = 0; i < root_array_size; i++) {
            jsmn_item_t root_ob_item;
            if (JSMN_GetArrayItem(&root, i, &root_ob_item) != 0)
                continue;

            jsmn_item_t root_ob_a_item;
            jsmn_item_t root_ob_b_item;
            if (JSMN_GetObjectItem(js, &root_ob_item, "a", &root_ob_a_item) != 0)
                continue;
            if (JSMN_GetObjectItem(js, &root_ob_item, "b", &root_ob_b_item) != 0)
                continue;

            char *item_string = JSMN_GetValueString(js, &root_ob_a_item);
            if (item_string == NULL)
                continue;

            LOG_I("get root array index:%d--->\"%s\":%s", i, JSMN_GetString(js, &root_ob_a_item), item_string);

            int b_array_size = JSMN_GetArraySize(&root_ob_b_item);
            if (b_array_size < 0)
                continue;

            for (int j = 0; j < b_array_size; j++) {
                jsmn_item_t b_ob_item;
                if (JSMN_GetArrayItem(&root_ob_b_item, j, &b_ob_item) != 0)
                    continue;

                jsmn_item_t b_ob_a_item;
                if (JSMN_GetObjectItem(js, &b_ob_item, "a", &b_ob_a_item) != 0)
                    continue;

                jsmn_item_t b_ob_a_value_item;
                jsmn_item_t b_ob_a_name_item;
                if (JSMN_GetObjectItem(js, &b_ob_a_item, "value", &b_ob_a_value_item) != 0)
                    continue;
                if (JSMN_GetObjectItem(js, &b_ob_a_item, "name", &b_ob_a_name_item) != 0)
                    continue;

                LOG_I("get root array index:%d--->\"%s\" array index:%d--->\"%s\":%s, \"%s\":%s", i, JSMN_GetString(js, &root_ob_b_item),
                      j, JSMN_GetString(js, &b_ob_a_value_item), JSMN_GetValueString(js, &b_ob_a_value_item), JSMN_GetString(js, &b_ob_a_name_item),
                      JSMN_GetValueString(js, &b_ob_a_name_item));
            }
        }
    } while (0);

    free(js);
    getchar();
    return 0;
}
