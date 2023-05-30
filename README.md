# web_server
最近实习写的简单web服务器。

## 说明
该程序是一个简单的HTTP服务器，它可以处理GET和POST请求，并支持多线程和多进程两种模式。

## 依赖

- C编译器
- POSIX线程库 (pthread)

## 编译

使用以下命令编译程序：

```bash
gcc -o http_server http_server.c http_request.c http_response.c http_utils.c config_handler.c -lpthread
```

或者使用项目中的Makefile：

```bash
make all
```

## 运行

运行编译后的可执行文件：

```bash
./http_server [配置文件]
```

默认情况下，程序将使用"config.txt"作为配置文件。

## 配置文件

配置文件是一个文本文件，用于指定服务器的根目录、根索引文件和端口号。

配置文件的格式如下：

```bash
coderoot_directory=/path/to/root
root_index=index.html
port=8080
```

其中：

- `root_directory` 指定服务器的根目录，即文件资源的根路径。
- `root_index` 指定在访问根路径时默认加载的文件。
- `port` 指定服务器监听的端口号。

## 处理请求

### GET请求

当接收到GET请求时，程序会根据请求的路径来确定要返回的文件。

- 如果路径是根路径 ("/")，将加载根索引文件。
- 如果路径包含查询字符串，程序会解析查询字符串，并打印每个键值对的内容。
- 根据文件路径的后缀名确定文件的类型，并返回相应的Content-Type头部字段。
- 支持的文件类型有：
  - .cgi: 执行CGI脚本并将结果返回。
  - .css: 返回CSS文件。
  - .js: 返回JavaScript文件。
  - .webp: 返回WebP图像文件。
  - 其他文件类型将返回默认的Content-Type ("text/html")。

### POST请求

当接收到POST请求时，程序会读取请求体数据，并根据Content-Length头部字段确定数据的大小。

- 如果请求体数据大于0，程序会打印请求体的内容。
- 如果请求体数据为文件上传，程序会将文件保存到服务器，并返回成功响应。

### 不支持的请求方法

如果收到除GET和POST之外的请求方法，程序将返回"405 Method Not Allowed"响应。

### 文件未找到

如果请求的文件不存在，程序将返回"404 Not Found"响应。

### 内部服务器错误

如果在处理请求时发生内部服务器错误，程序将返回"500 Internal Server Error"响应。

## 多线程模式

可以使用多线程模式启动HTTP服务器，以同时处理多个客户端请求。

启动多线程模式的方法是调用`start_web_server_mutil_thread`函数，并传入根目录、根索引和端口号作为参数。

## 多进程模式

可以使用多进程模式启动HTTP服务器，以同时处理多个客户端请求。

启动多进程模式的方法是调用`start_web_server_multi_process`函数，并传入根目录、根索引和端口号作为参数。

在多进程模式下，每个客户端请求将由一个独立的子进程处理。

## 示例

1. 使用默认配置文件以多线程模式运行服务器：

```bash
./http_server -t
```

2. 使用自定义配置文件以多进程模式运行服务器：

```bash
./http_server custom_config.txt -p
```

## 
