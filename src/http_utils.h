#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H

/**
 * 处理上传文件
 * @param client_sock 客户端套接字
 * @param file_data 文件数据
 * @param data_length 文件数据长度
*/
void handle_file_upload(int client_sock, const char *file_data, int data_length);

#endif // !HTTP_UTILS_H