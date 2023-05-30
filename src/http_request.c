#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "http_request.h"
#include "http_response.h"
#include "http_utils.h"

#define BUFFER_SIZE 1024
#define MAX_REQUEST_SIZE 2048
#define DEFAULT_CONTENT_TYPE "text/html"
#define GET_METHOD "GET"
#define POST_METHOD "POST"
#define PUT_METHOD "PUT"
#define DELETE_METHOD "DELETE"

void handle_request(int client_sock, const char *root_directory, const char *root_index)
{
    char buffer[MAX_REQUEST_SIZE];
    memset(buffer, 0, sizeof(buffer));

    int bytes_read = read(client_sock, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0)
    {
        perror("Failed to read request");
        internal_server_error(client_sock);
        return;
    }

    char *method = strtok(buffer, " ");
    char *path = strtok(NULL, " ");

    if (strcmp(method, GET_METHOD) == 0)
    {
        // 处理GET请求
        if (strcmp(path, "/") == 0)
        {
            path = (char *)malloc(strlen(root_index) + 1);
            strcpy(path, root_index);
        }

        // 检查是否包含查询字符串
        char *query_string = strchr(path, '?');
        if (query_string != NULL)
        {
            // 将查询字符串分割成键值对
            query_string++; // 跳过问号
            char *pair = strtok(query_string, "&");
            while (pair != NULL)
            {
                // 解析键值对
                char *key = strtok(pair, "=");
                char *value = strtok(NULL, "=");

                // 打印获取到的键值对数据
                printf("Key: %s, Value: %s\n", key, value);

                // 继续处理下一个键值对
                pair = strtok(NULL, "&");
            }
        }

        // 拼接文件路径
        char file_path[BUFFER_SIZE];
        sprintf(file_path, "%s%s", root_directory, path);

        if (access(file_path, F_OK) == 0)
        {
            if (strstr(file_path, ".cgi") != NULL)
            {
                char *envp[] = {NULL};
                char *argv[] = {file_path, NULL};
                dup2(client_sock, STDOUT_FILENO);
                execve(file_path, argv, envp);
            }
            else if (strstr(file_path, ".css") != NULL)
            {
                send_file(client_sock, file_path, "text/css");
            }
            else if (strstr(file_path, ".js") != NULL)
            {
                send_file(client_sock, file_path, "text/javascript");
            }
            else
            {
                send_file(client_sock, file_path, DEFAULT_CONTENT_TYPE);
            }
        }
        else
        {
            not_found(client_sock);
        }
    }
    else if (strcmp(method, POST_METHOD) == 0)
    {
        // 处理POST请求

        // 读取请求体数据
        char body[MAX_REQUEST_SIZE];
        memset(body, 0, sizeof(body));
        int content_length = 0;

        // 查找Content-Length头部字段，以确定请求体的大小
        char *header;
        while ((header = strtok(NULL, "\r\n")) != NULL)
        {
            if (strstr(header, "Content-Length: ") == header)
            {
                content_length = atoi(header + strlen("Content-Length: "));
                break;
            }
        }

        // 读取请求体数据
        if (content_length > 0)
        {
            int bytes_read = read(client_sock, body, content_length);
            if (bytes_read < 0)
            {
                perror("Failed to read request body");
                internal_server_error(client_sock);
                return;
            }
        }

        // TODO: 在这里可以对请求体数据进行处理，例如解析表单数据、处理上传文件等

        // 处理文件上传
        // handle_file_upload(client_sock, body, content_length);
        //  发送响应
        const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nPOST Request Received";
        send_response(client_sock, response);
    }
    else if (strcmp(method, PUT_METHOD) == 0)
    {
        // 处理PUT请求
        method_not_allowed(client_sock);
        return;
    }
    else if (strcmp(method, DELETE_METHOD) == 0)
    {
        // 处理DELETE请求
        method_not_allowed(client_sock);
        return;
    }
    else
    {
        // 不支持的请求方法
        not_found(client_sock);
        return;
    }
}