#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <sys/stat.h>

#include "http_server.h"
#include "http_request.h"

const char *_root_dir;
const char *_index;

void *handle_client(void *arg)
{
    int client_sock = *(int *)arg;
    handle_request(client_sock, _root_dir, _index);
    close(client_sock);
    free(arg);
    return NULL;
}

void start_web_server_multithread(const char *root_directory, const char *root_index, int port)
{
    _root_dir = root_directory;
    _index = root_index;

    int listen_sock;
    struct sockaddr_in server_addr;

    if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Failed to create listen socket");
        exit(1);
    }

    int reuse = 1;
    if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        perror("Failed to set SO_REUSEADDR");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(listen_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Failed to bind listen socket");
        exit(1);
    }

    if (listen(listen_sock, SOMAXCONN) < 0)
    {
        perror("Failed to listen on socket");
        exit(1);
    }

    printf("Server is running on port %d\n", port);

    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_sock = accept(listen_sock, (struct sockaddr *)&client_addr, &client_addr_len);

        if (client_sock < 0)
        {
            perror("Failed to accept connection");
            continue;
        }

        printf("Accepted connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // 多线程处理请求
        int *client_sock_ptr = (int *)malloc(sizeof(int));
        *client_sock_ptr = client_sock;

        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, (void *)client_sock_ptr) != 0)
        {
            perror("Failed to create thread");
            close(client_sock);
            free(client_sock_ptr);
            continue;
        }

        pthread_detach(thread);
    }

    close(listen_sock);
}

void start_web_server_multiprocess(const char *root_directory, const char *root_index, int port)
{
    // Fork第一个子进程
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("无法fork第一个子进程");
        exit(1);
    }
    else if (pid > 0)
    {
        // 退出父进程
        exit(0);
    }

    // 创建新会话并成为新会话的领导者
    if (setsid() < 0)
    {
        perror("无法创建新会话");
        exit(1);
    }

    // 再次fork以确保子进程无法在未来获取新的控制终端
    pid = fork();

    if (pid < 0)
    {
        perror("无法fork第二个子进程");
        exit(1);
    }
    else if (pid > 0)
    {
        // 退出第一个子进程
        exit(0);
    }

    // 关闭标准输入、输出和错误输出文件描述符
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // 清除文件模式创建掩码
    umask(0);

    // 关闭不需要的打开文件描述符
    int max_fd = sysconf(_SC_OPEN_MAX);
    for (int fd = 0; fd < max_fd; fd++)
    {
        close(fd);
    }

    int listen_sock;
    struct sockaddr_in server_addr;

    if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("无法创建监听套接字");
        exit(1);
    }

    int reuse = 1;
    if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        perror("Failed to set SO_REUSEADDR");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(listen_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Failed to bind listen socket");
        exit(1);
    }

    if (listen(listen_sock, SOMAXCONN) < 0)
    {
        perror("Failed to listen on socket");
        exit(1);
    }

    printf("Server is running on port %d\n", port);

    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_sock = accept(listen_sock, (struct sockaddr *)&client_addr, &client_addr_len);

        if (client_sock < 0)
        {
            perror("Failed to accept connection");
            continue;
        }

        printf("Accepted connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        pid_t pid = fork();
        if (pid == 0)
        {
            close(listen_sock);
            handle_request(client_sock, root_directory, root_index);
            close(client_sock);
            exit(0);
        }
        else if (pid > 0)
        {
            close(client_sock);
        }
        else
        {
            perror("Failed to create child process");
            close(client_sock);
        }
    }

    close(listen_sock);
}
