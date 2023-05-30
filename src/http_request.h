#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

/**
 * 处理请求
 * @param client_sock 客户端套接字
*/
void handle_request(int client_sock, const char *root_directory, const char *root_index);

#endif // HTTP_REQUEST_H