#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

// 以下函数为多线程Web服务器模块
/**
 * 处理客户端请求(线程处理函数)
 * @param arg 客户端套接字
*/
void *handle_client(void *arg);

/**
 * 启动 Web 服务器(多线程版本)
 * @param root_directory 网站根目录
 * @param root_index 根目录的默认文件
 * @param port 端口
*/
void start_web_server_multithread(const char *root_directory, const char *root_index, int port);

// 以下函数为多进程Web服务器模块
/**
 * 启动 Web 服务器(多进程版本)
 * @param root_directory 网站根目录
 * @param root_index 根目录的默认文件
 * @param port 端口
*/
void start_web_server_multiprocess(const char *root_directory, const char *root_index, int port);

#endif // !HTTP_SERVER_H