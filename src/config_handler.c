#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config_handler.h"

int parse_config(const char *config_file, char *root_directory, char *root_index, int *port)
{
    FILE *file = fopen(config_file, "r");
    if (file == NULL)
    {
        printf("Failed to open configuration file: %s\n", config_file);
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        // 去除换行符
        line[strcspn(line, "\n")] = '\0';

        // 解析配置项
        char key[100];
        char value[100];

        if (sscanf(line, "%[^=]=%s", key, value) == 2)
        {
            if (strcmp(key, "root_directory") == 0)
            {
                strcpy(root_directory, value);
            }
            else if (strcmp(key, "root_index") == 0)
            {
                strcpy(root_index, value);
            }
            else if (strcmp(key, "port") == 0)
            {
                *port = atoi(value);
            }
        }
    }

    fclose(file);
    return 0;
}