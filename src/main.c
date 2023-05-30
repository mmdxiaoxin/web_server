#include <stdio.h>
#include "http_server.h"
#include "config_handler.h"

int main(int argc, char *argv[])
{
    // 从配置文件读取参数
    char config_file[] = "config.txt";
    char root_directory[100];
    char root_index[50];
    int port;

    if (parse_config(config_file, root_directory, root_index, &port) == -1)
    {
        printf("无法读取配置文件！\n");
        return -1;
    }

    start_web_server(root_directory, root_index, port);

    return 0;
}
