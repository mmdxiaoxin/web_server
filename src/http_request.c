#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>

#include "http_request.h"
#include "http_response.h"
#include "http_utils.h"

#define BUFFER_SIZE 1024
#define MAX_REQUEST_SIZE 2048
#define MAX_HEADER_SIZE 2048
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

    if (method == NULL || path == NULL)
    {
        // 请求行为空或方法和路径为空，处理错误
        not_found(client_sock);
        return;
    }

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
            if (strstr(file_path, ".css") != NULL)
            {
                printf("Sending CSS file\n");
                send_file(client_sock, file_path, "text/css");
            }
            else if (strstr(file_path, ".js") != NULL)
            {
                printf("Sending JS file\n");
                send_file(client_sock, file_path, "text/javascript");
            }
            else if (strstr(file_path, ".webp") != NULL)
            {
                printf("Sending WebP file\n");
                send_file(client_sock, file_path, "image/webp");
            }
            else
            {
                printf("Sending HTML file\n");
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
        char *header;
        char request_headers[MAX_HEADER_SIZE]; // 存储请求头部字段
        memset(request_headers, 0, sizeof(request_headers));
        while ((header = strtok(NULL, "\r\n")) != NULL)
        {
            strcat(request_headers, header);
            strcat(request_headers, "\r\n"); // 添加换行符
        }
        // 获取Content-Length和Content-Type头部字段的值
        char *content_length_str = get_header_value(request_headers, "Content-Length: ");
        char *content_type = get_header_value(request_headers, "Content-Type: ");

        // 检查是否存在Content-Length和Content-Type头部字段
        if (content_length_str == NULL || content_type == NULL)
        {
            bad_request(client_sock);
            free(content_length_str);
            free(content_type);
            return;
        }

        // 将Content-Length字符串转换为整数
        int content_length = atoi(content_length_str);
        free(content_length_str);

        // 检查Content-Type是否为multipart/form-data
        if (strncmp(content_type, "multipart/form-data", 19) != 0)
        {
            not_found(client_sock);
            free(content_type);
            return;
        }

        // 获取分隔符
        char *boundary_start = strstr(content_type, "boundary=");
        if (boundary_start == NULL)
        {
            bad_request(client_sock);
            free(content_type);
            return;
        }

        char *boundary = boundary_start + 9;
        size_t boundary_len = strlen(boundary);

        // 读取请求体数据
        char *body_data = malloc(content_length + 1);
        if (body_data == NULL)
        {
            internal_server_error(client_sock);
            free(content_type);
            return;
        }

        //获取请求体数据长度
        ssize_t total_bytes_read = 0;
        ssize_t bytes_read;
        while (total_bytes_read < content_length)
        {
            bytes_read = recv(client_sock, body_data + total_bytes_read, content_length - total_bytes_read, 0);
            if (bytes_read == -1)
            {
                internal_server_error(client_sock);
                free(content_type);
                free(body_data);
                return;
            }
            if (bytes_read == 0)
            {
                break; // 连接已关闭
            }
            total_bytes_read += bytes_read;
        }

        if (total_bytes_read != content_length)
        {
            internal_server_error(client_sock);
            free(content_type);
            free(body_data);
            return;
        }

        body_data[content_length] = '\0';

        // 解析请求体数据，提取文件数据
        char *file_data_start = strstr(body_data, "\r\n\r\n") + 4;
        if (file_data_start == NULL)
        {
            bad_request(client_sock);
            free(content_type);
            free(body_data);
            return;
        }

        int file_data_length = content_length - (file_data_start - body_data) - boundary_len - 6;

        // 处理文件上传
        handle_file_upload(client_sock, file_data_start, file_data_length);

        // 释放内存
        free(content_type);
        free(body_data);
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