#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <limits.h>

int datagram_size = 65507; // for IPv4

int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(2020);

    int server_fd;

    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    char bufor[datagram_size + 1];
    char odpowiedz[datagram_size];
    unsigned int suma = 0;
    int przekonwertowana;
    char odczytana[1];

    while (1)
    {
        struct sockaddr_in klient;
        socklen_t klientRozmiar = sizeof(klient);

        if (recvfrom(server_fd, bufor, sizeof(bufor), 0, (struct sockaddr *)&klient, &klientRozmiar) == -1)
        {
            perror("recvfrom() failed");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < datagram_size; i++)
        {

            if (bufor[i] >= '0' && bufor[i] <= '9') // nie trafiliśmy na spacje ani znak końca linii
            {
                odczytana[0] = bufor[i] - '0';
                przekonwertowana = atoi(odczytana);
                suma += przekonwertowana;
            }
            // możemy napotkać: spacje, \n, lub \r\n
            else if (bufor[i] == ' ' || bufor[i] == '\n' || (bufor[i] == '\r' && bufor[i + 1] == '\n'))
            {
            }
        }

        //konwertujemy sume na string
        int dlugosc_odpowiedzi = sprintf(odpowiedz, "%d\n", suma);

        // wysyłamy dane do klienta
        if (sendto(server_fd, odpowiedz, dlugosc_odpowiedzi, 0, (struct sockaddr *)&klient, klientRozmiar) == -1)
        {
            perror("sendto() failed");
            exit(EXIT_FAILURE);
        }
    }

    if (close(server_fd) == -1)
    {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return 0;
}