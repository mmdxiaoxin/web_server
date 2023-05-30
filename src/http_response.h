#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

/**
 * 发送响应
 * @param client_sock 客户端套接字
 * @param response 响应内容
*/
void send_response(int client_sock, const char *response);

/**
 * 发送文件
 * @param client_sock 客户端套接字
 * @param file_path 文件路径
 * @param content_type 内容类型
*/
void send_file(int client_sock, const char *file_path, const char *content_type);

/**
 * 404 Not Found
 * @param client_sock 客户端套接字
*/
void not_found(int client_sock);

/**
 * 500 Internal Server Error
 * @param client_sock 客户端套接字
*/
void internal_server_error(int client_sock);

/**
 * 405 Method Not Allowed
 * @param client_sock 客户端套接字
*/
void method_not_allowed(int client_sock);

#endif // HTTP_RESPONSE_H