#ifndef CONFIG_HANDLER_H
#define CONFIG_HANDLER_H

/**
 * 解析配置文件
 * @param config_file 配置文件路径
 * @param root_directory 网站根目录
 * @param root_index 网站根索引
 * @param port 端口
 * @return 0: 成功 -1: 失败
*/
int parse_config(const char *config_file, char *root_directory, char *root_index, int *port);

#endif // !CONFIG_HANDLER_H