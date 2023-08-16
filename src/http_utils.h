#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H

// 以下函数为文件上传处理模块
/**
 * 处理上传文件
 * @param client_sock 客户端套接字
 * @param file_data 文件数据
 * @param data_length 文件数据长度
 */
void handle_file_upload(int client_sock, const char *file_data, int data_length);

/**
 * 获取头部字段的值
 * @param headers 头部字段字符串
 * @param header_name 头部字段名称
 * @return 头部字段的值
 */
char *get_header_value(const char *headers, const char *header_name);

#endif // !HTTP_UTILS_H