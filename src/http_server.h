#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

/**
 * 处理客户端请求(线程处理函数)
 * @param arg 客户端套接字
*/
void *handle_client(void *arg);

/**
 * 启动 Web 服务器(多线程版本)
 * @param root_directory 网站根目录
 * @param root_index 网站根索引
 * @param port 端口
*/
void start_web_server_mutil_thread(const char *root_directory, const char *root_index, int port);

/**
 * 启动 Web 服务器(多进程版本)
 * @param root_directory 网站根目录
 * @param root_index 网站根索引
 * @param port 端口
*/
void start_web_server_multi_process(const char *root_directory, const char *root_index, int port);

#endif // !HTTP_SERVER_H