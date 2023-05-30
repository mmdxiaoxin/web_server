#include <stdio.h>

#include "http_server.h"
#include "config_handler.h"

#define DEFAULT_CONFIG_FILE "config.txt"

int main(int argc, char *argv[])
{
    // 从配置文件读取参数
    char config_file[256];

    if (argc >= 2)
    {
        strncpy(config_file, argv[1], sizeof(config_file) - 1);
    }
    else
    {
        strncpy(config_file, DEFAULT_CONFIG_FILE, sizeof(config_file) - 1);
    }

    char root_directory[100];
    char root_index[50];
    int port;

    if (parse_config(config_file, root_directory, root_index, &port) == -1)
    {
        printf("无法读取配置文件！\n");
        return -1;
    }

    start_web_server_mutil_thread(root_directory, root_index, port);
    // start_web_server_multi_process(root_directory, root_index, port);

    return 0;
}
