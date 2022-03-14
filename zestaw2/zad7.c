#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        perror("Podano bledna ilosc argumentow wywolania\n");
        exit(EXIT_FAILURE);
    }

    int IPv4 = atoi(argv[1]);
    int PORT = atoi(argv[2]);

    int client_fd;
    struct sockaddr_in server_addr;
    char message[64];

    // Clean buffers:
    memset(message, '\0', sizeof(message));

    // Create socket:
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set port and IP the same as server-side:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htons(IPv4);

    // Send connection request to server:
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    // Receive the server's response:
    if (recv(client_fd, message, sizeof(message), 0) < 0)
    {
        perror("recv failed");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < sizeof(message); i++)
    {
        putchar(message[i]);
    }
    printf("\n");

    // Close the socket:
    if (close(client_fd) == -1)
    {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return 0;
}