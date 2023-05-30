#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

/**
 * 启动 Web 服务器
 * @param root_directory 网站根目录
 * @param root_index 网站根索引
 * @param port 端口
*/
void start_web_server(const char *root_directory, const char *root_index, int port);

#endif // !HTTP_SERVER_H