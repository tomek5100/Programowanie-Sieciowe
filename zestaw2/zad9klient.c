#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>

bool drukowalne(const void *buf, int len)
{
    const char *buffer = buf;

    for (int i = 0; i < len; i++)
    {
        if (buffer[i] < 32 || buffer[i] > 126)
            return false;
    }
    return true;
}

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        perror("Podano bledna ilosc argumentow wywolania\n");
        exit(EXIT_FAILURE);
    }

    const char *IPv4 = argv[1];
    int PORT = atoi(argv[2]);

    int client_fd;
    struct sockaddr_in server_addr;

    // Set port and IP the same as server-side:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IPv4);

    // Create socket:
    if ((client_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // wysyłamy pusty datagram
    if (sendto(client_fd, "", 0, 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) == -1)
    {
        perror("sendto failed");
        exit(EXIT_FAILURE);
    }

    char message[64];
    // Clean buffers:
    memset(message, '\0', sizeof(message));

    socklen_t adresRozmiar = sizeof(server_addr);

    // Read the server's message:
    int readed;
    if ((readed = recvfrom(client_fd, message, 32, 0, (struct sockaddr *)&server_addr, &adresRozmiar)) == -1)
    {
        perror("recvfrom failed");
        exit(EXIT_FAILURE);
    }

    if (drukowalne(message, readed - 2))
    {
        printf("%s\n", message);
    }
    else
    {
        printf("Odebrano niedrukowalne znaki\n");
        exit(EXIT_FAILURE);
    }

    if (close(client_fd) == -1)
    {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return 0;
}