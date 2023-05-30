#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "http_utils.h"
#include "http_response.h"

void handle_file_upload(int client_sock, const char *file_data, int data_length)
{
    // 生成一个唯一的文件名
    char file_name[20];
    sprintf(file_name, "file_%d", rand());

    // 在服务器上创建文件
    FILE *file = fopen(file_name, "wb");
    if (file == NULL)
    {
        perror("Failed to create file");
        internal_server_error(client_sock);
        return;
    }

    // 将文件数据写入到服务器文件
    size_t bytes_written = fwrite(file_data, 1, data_length, file);
    if (bytes_written != (size_t)data_length)
    {
        perror("Failed to write file");
        fclose(file);
        internal_server_error(client_sock);
        return;
    }

    // 关闭文件
    fclose(file);

    // 发送成功响应
    const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nFile Uploaded";
    send_response(client_sock, response);
}
