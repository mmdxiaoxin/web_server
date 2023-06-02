#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

// 以下函数为HTTP请求处理模块
/**
 * 处理请求
 * @param client_sock 客户端套接字
 * @param root_directory 网站根目录
 * @param root_index 根目录的默认文件
*/
void handle_request(int client_sock, const char *root_directory, const char *root_index);

#endif // HTTP_REQUEST_H