#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

int datagram_size = 65507; // for IPv4

bool drukowalne(const char *buf)
{
    int iter = 0;
    while (1)
    {
        if (*buf == 0)
        {
            if (iter > 0) // zabezpieczenie przed datagramem posiadajacym np. same spacje
                return true;
            else
                return false;
        }
        if (*buf < 48 || *buf > 57)
        {
            if (*(buf) == 13 || *buf == 10 || *buf == 32)
            {
                buf++;
                continue;
            }
            return false;
        }
        iter++;
        buf++;
    }
}

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
        bool overload = false;
        bool not_number = false;
        bool is_empty = false;
        int recvfrom_size = 0;
        char bufor[datagram_size + 1];
        memset(bufor, 0, sizeof(bufor));
        char odpowiedz[datagram_size];
        memset(odpowiedz, 0, sizeof(odpowiedz));
        struct sockaddr_in klient;
        socklen_t klientRozmiar = sizeof(klient);

        if ((recvfrom_size = recvfrom(server_fd, bufor, sizeof(bufor), 0, (struct sockaddr *)&klient, &klientRozmiar)) == -1)
        {
            perror("recvfrom() failed");
            exit(EXIT_FAILURE);
        }

        unsigned int suma = 0;
        char odczytana[20];
        memset(odczytana, 0, sizeof(odczytana));
        int i = 0;
        const char *bufor_pom = bufor;

        if (recvfrom_size == 0)
        {
            printf("Otrzymany datagram jest pusty\n");
            is_empty = true;
        }
        else if (drukowalne(bufor) == false)
        {
            printf("Otrzymane znaki nie sa cyframi\n");
            not_number = true;
        }
        else
        {
            printf("odczytano: %s", bufor);

            while (*bufor_pom != 0)
            {

                if (*bufor_pom == 13 || *bufor_pom == 10)
                {
                    bufor_pom++;
                    continue;
                }
                if (*bufor_pom == ' ')
                {
                    suma += atoi(odczytana);
                    memset(odczytana, 0, sizeof(odczytana));
                    i = 0;
                }
                odczytana[i] = *bufor_pom;
                // printf("odczytana: %c\n", odczytana[i]);
                // printf("tutaj\n");
                i++;
                bufor_pom++;
            }

            long cast = strtol(odczytana, 0, 10);
            if((cast == LONG_MAX) || (cast == LONG_MIN)){
                printf("Przekroczono zakres long podczas konwersji\n");
                overload = true;
            }

            // printf("strtol %ld\n", cast);

            if (suma > INT_MAX - cast)
            {
                printf("OVERLOAD\n");
                overload = true;
            }
            suma += atoi(odczytana);
            memset(odczytana, 0, sizeof(odczytana));
        }

        int dlugosc_odpowiedzi = 0;

        if ((overload == true) || (not_number == true) || (is_empty == true))
        {
            dlugosc_odpowiedzi = sprintf(odpowiedz, "ERROR");
        }
        else
        {
            dlugosc_odpowiedzi = sprintf(odpowiedz, "%d\n", suma);
        }

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