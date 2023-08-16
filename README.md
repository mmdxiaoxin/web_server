# 程序说明

该程序是一个简单的WEB服务器，支持处理GET和POST请求，并提供多线程和多进程两种模式。

## 依赖

- C编译器（GCC)
- POSIX线程库 (pthread)
- Linux环境

## 编译

使用以下命令编译程序：

```bash
gcc main.c -pthread -std=c99 -o web_server
```

或者使用项目中的Makefile：

```bash
make all
```

## 运行

运行编译后的可执行文件：

```bash
./web_server [选项] <配置文件>
```

选项:

- `-p` 使用多进程模式
- `-c <文件路径>` 指定配置文件

默认情况下，程序将使用可执行文件所在目录的`"config.txt"`作为配置文件，并以多线程模式启动。

## 停止

使用`<Ctrl+C>`即可关闭多线程服务。多进程服务可以使用`stop_web_server.sh`脚本快速关闭。

## 配置文件

配置文件是一个文本文件，用于指定服务器的根目录、根索引文件和端口号。

配置文件的格式如下：

```bash
root_directory=/path/to/root
root_index=index.html
port=8080
```

其中：

- `root_directory` 指定服务器的根目录，即文件资源的根路径。
- `root_index` 指定在访问根路径时默认加载的文件。
- `port` 指定服务器监听的端口号。

## 处理请求

### GET请求

当接收到GET请求时，程序根据请求的路径来确定要返回的文件。

- 如果路径是根路径 ("/")，将加载根索引文件。
- 如果路径包含查询字符串，程序解析查询字符串，并打印每个键值对的内容。
- 根据文件路径的后缀名确定文件的类型，并返回相应的Content-Type头部字段。
- 支持的文件类型有：
  - `.css` 返回CSS文件。
  - `.js` 返回JavaScript文件。
  - `.webp` 返回WebP图像文件。
  - 其他文件类型将返回默认的Content-Type ("text/html")即：`.html`。

### POST请求

当接收到POST请求时，程序读取请求体数据，并根据Content-Length头部字段确定数据的大小。

- 如果请求体数据大于0，程序打印请求体的内容。
- 如果请求体数据是文件上传，程序将文件保存到服务器，并返回成功响应。

### 不支持的请求方法

如果收到除GET和POST之外的请求方法，程序返回"405 Method Not Allowed"响应。

### 文件未找到

如果请求的文件不存在，程序返回"404 Not Found"响应。

### 内部服务器错误

如果在处理请求时发生内部服务器错误，程序返回"500 Internal Server Error"响应。

## 多线程模式

可以使用多线程模式启动HTTP服务器，以同时处理多个客户端请求。

启动多线程模式的方法是调用`start_web_server_mutilthread`函数，并传入根目录、根索引和端口号作为参数。

## 多进程模式

可以使用多进程模式启动HTTP服务器，以同时处理多个客户端请求。

启动多进程模式的方法是调用`start_web_server_multiprocess`函数，并传入根目录、根索引和端口号作为参数。

在多进程模式下，每个客户端请求将由一个独立的子进程处理。

## 示例

1. 使用默认配置文件以多线程模式运行服务器：

```bash
./web_server
```

2. 使用自定义配置文件以多进程模式运行服务器：

```bash
./web_server -p -c /path/custom_config.txt
```

## 代码描述（主要函数）：

- `config_handler.h`：包含`parse_config`函数，用于解析配置文件并将配置项的值保存在传入的参数中。
- `http_request.h`：包含`handle_request`函数，用于处理客户端发来的HTTP请求。根据请求的方法和路径执行相应的操作，如返回文件内容、处理文件上传等。
- `http_response.h`：包含用于发送HTTP响应的函数，如`send_response`用于发送响应头部和正文，`send_file`用于发送文件内容，以及一些特定响应的函数，如`not_found`用于返回404错误，`internal_server_error`用于返回500错误等。
- `http_server.h`：包含`start_web_server_multithread`和`start_web_server_multiprocess`两个函数，用于启动HTTP服务器。`start_web_server_multithread`使用多线程处理请求，而`start_web_server_multiprocess`使用多进程处理请求。
- `http_utils.h`：包含一些与HTTP相关的实用函数，如用于解析URL参数的函数。

## 其他注意事项

- 该程序仅供演示和学习目的，不适用于生产环境。
- 在运行程序之前，请确保已安装所需的依赖项并正确配置编译环境。
- 在运行程序之前，请确保配置文件的路径和内容正确。
- 在运行程序之前，请确保服务器根目录下存在所需的文件。
