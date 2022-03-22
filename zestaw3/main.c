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

    while (1)
    {
        char bufor[datagram_size + 1];
        memset(bufor, 0, sizeof(bufor));
        char odpowiedz[datagram_size];
        memset(odpowiedz, 0, sizeof(odpowiedz));
        struct sockaddr_in klient;
        socklen_t klientRozmiar = sizeof(klient);

        if (recvfrom(server_fd, bufor, sizeof(bufor), 0, (struct sockaddr *)&klient, &klientRozmiar) == -1)
        {
            perror("recvfrom() failed");
            exit(EXIT_FAILURE);
        }

        printf("odczytano: %s", bufor);

        unsigned int suma = 0;
        char odczytana[10];
        int i = 0;
        const char *bufor_pom = bufor;

        while (*bufor_pom != 0)
        {
            if (*bufor_pom == ' ')
            {
                suma += atoi(odczytana);
                memset(odczytana, 0, sizeof(odczytana));
                i = 0;
            }
            odczytana[i] = *bufor_pom;
            i++;
            bufor_pom++;
        }
        suma += atoi(odczytana);
        memset(odczytana, 0, sizeof(odczytana));

        int dlugosc_odpowiedzi = sprintf(odpowiedz, "%d\n", suma);

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