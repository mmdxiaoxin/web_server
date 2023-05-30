#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

void send_response(int client_sock, const char *response)
{
    write(client_sock, response, strlen(response));
}

void send_file(int client_sock, const char *file_path, const char *content_type)
{
    int file_fd = open(file_path, O_RDONLY);
    if (file_fd < 0)
    {
        perror("Failed to open file");
        internal_server_error(client_sock);
        return;
    }

    off_t file_size = lseek(file_fd, 0, SEEK_END);
    lseek(file_fd, 0, SEEK_SET);

    char header[BUFFER_SIZE];
    sprintf(header, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %ld\r\n\r\n", content_type, file_size);
    write(client_sock, header, strlen(header));

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(file_fd, buffer, sizeof(buffer))) > 0)
    {
        write(client_sock, buffer, bytes_read);
    }

    close(file_fd);
}

void not_found(int client_sock)
{
    char response[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n404 Not Found";
    send_response(client_sock, response);
}

void internal_server_error(int client_sock)
{
    char response[] = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/plain\r\n\r\n500 Internal Server Error";
    send_response(client_sock, response);
}