#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define MAX_BUFFER 1024

int main() {
    char buffer[MAX_BUFFER];
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("webserver (socket)");
        exit(EXIT_FAILURE);
    }
    printf("Socket created sucessfully!");

    struct sockaddr_in host_address;
    unsigned host_address_len = sizeof(host_address);
    host_address.sin_family = AF_INET;
    host_address.sin_port = htons(PORT);
    host_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr *)&host_address, host_address_len) != 0) {
        perror("webserver (bind)");
        exit(EXIT_FAILURE);
    }
    printf("Socket sucessfully binded to address");

    if (listen(sockfd, SOMAXCONN) != 0) {
        perror("webserver (listen)");
        exit(EXIT_FAILURE);
    }
    printf("Server listening for connections");

    for (;;) {
        int newsockfd = accept(sockfd, (struct sockaddr *)&host_address, (unsigned *)&host_address_len);
        if (newsockfd < 0) {
            perror("webserver (accept)");
            continue;
        }
        printf("connection accepted");

        int valread = read(newsockfd, buffer, MAX_BUFFER);
        if (valread < 0) {
            perror("webserver (read)");
            continue;
        }

        char resp[] = "HTTP/1.0 200 OK\r\n"
                      "Server: webserver-c\r\n"
                      "Content-type: text/html\r\n\r\n"
                      "<html>hello, world</html>\r\n";
        size_t resplen = strlen(resp);

        int valwrite = write(newsockfd, resp, resplen);
        if (valwrite < 0) {
            perror("webserver (write)");
            continue;
        }
        close(newsockfd);
    }
    exit(EXIT_SUCCESS);
}
