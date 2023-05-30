#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config_handler.h"

int parse_config(const char *config_file, char *root_directory, char *root_index, int *port)
{
    FILE *file;
    char line[100];
    char key[20];
    char value[80];

    // 打开配置文件
    if ((file = fopen(config_file, "r")) == NULL)
    {
        return -1;
    }

    // 读取配置文件中的参数
    while (fgets(line, sizeof(line), file) != NULL)
    {
        sscanf(line, "%s %s", key, value);

        if (strcmp(key, "RootDirectory") == 0)
        {
            strcpy(root_directory, value);
        }
        else if (strcmp(key, "DefaultIndex") == 0)
        {
            strcpy(root_index, value);
        }
        else if (strcmp(key, "Port") == 0)
        {
            *port = atoi(value);
        }
    }

    fclose(file);
    return 0;
}