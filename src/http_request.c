#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "http_request.h"
#include "http_response.h"

#define BUFFER_SIZE 1024
#define MAX_REQUEST_SIZE 2048
#define DEFAULT_CONTENT_TYPE "text/html"

void handle_request(int client_sock, const char *root_directory, const char *root_index)
{
    char buffer[MAX_REQUEST_SIZE];
    memset(buffer, 0, sizeof(buffer));

    int bytes_read = read(client_sock, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0)
    {
        perror("Failed to read request");
        internal_server_error(client_sock);
        return;
    }

    char *method = strtok(buffer, " ");
    char *path = strtok(NULL, " ");

    if (strcmp(method, "GET") == 0)
    {
        if (strcmp(path, "/") == 0)
        {
            path = (char *)malloc(strlen(root_index) + 1);
            strcpy(path, root_index);
        }

        char file_path[BUFFER_SIZE];
        sprintf(file_path, "%s%s", root_directory, path);

        if (access(file_path, F_OK) == 0)
        {
            if (strstr(file_path, ".cgi") != NULL)
            {
                char *envp[] = {NULL};
                char *argv[] = {file_path, NULL};
                dup2(client_sock, STDOUT_FILENO);
                execve(file_path, argv, envp);
            }
            else if (strstr(file_path, ".css") != NULL)
            {
                send_file(client_sock, file_path, "text/css");
            }
            else if (strstr(file_path, ".js") != NULL)
            {
                send_file(client_sock, file_path, "text/javascript");
            }
            else
            {
                send_file(client_sock, file_path, DEFAULT_CONTENT_TYPE);
            }
        }
        else
        {
            not_found(client_sock);
        }
    }
    else
    {
        not_found(client_sock);
    }
}