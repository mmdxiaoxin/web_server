#include <stdio.h>
#include <string.h>

#include "http_server.h"
#include "config_handler.h"

#define DEFAULT_CONFIG_FILE "config.txt"

enum // 服务器模式
{
    MODE_MULTITHREAD, // 多线程模式
    MODE_MULTIPROCESS // 多进程模式
};

int main(int argc, char *argv[])
{
    // 默认参数
    char config_file[256] = DEFAULT_CONFIG_FILE;
    int mode = MODE_MULTITHREAD;

    // 解析命令行参数
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-p") == 0)
        {
            mode = MODE_MULTIPROCESS;
        }
        else if (strcmp(argv[i], "-c") == 0 && i + 1 < argc)
        {
            strncpy(config_file, argv[i + 1], sizeof(config_file) - 1);
            i++; // 跳过下一个参数
        }
        else
        {
            printf("无效的选项或参数：%s\n", argv[i]);
            printf("用法: %s [配置文件] [选项]\n", argv[0]);
            printf("选项:\n");
            printf("  -p              使用多进程模式\n");
            printf("  -c <文件路径>   指定配置文件\n");
            return -1;
        }
    }

    // 从配置文件读取参数
    char root_directory[100];
    char root_index[50];
    int port;

    if (parse_config(config_file, root_directory, root_index, &port) == -1)
    {
        printf("无法读取配置文件！\n");
        return -1;
    }

    // 根据模式启动服务器
    if (mode == MODE_MULTITHREAD)
    {
        start_web_server_multithread(root_directory, root_index, port);
    }
    else if (mode == MODE_MULTIPROCESS)
    {
        start_web_server_multiprocess(root_directory, root_index, port);
    }

    return 0;
}
