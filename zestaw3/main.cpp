#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <limits.h>
#include <iostream>
#include <conio.h>

int datagram_size = 65507; //for IPv4

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        perror("Podano bledna ilosc argumentow wywolania\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(atoi(argv[1]));

    int server_fd;

    /*
    AF_INET - obsługa protokołu IPv4
    SOCK_DGRAM -  odnosi się do protokołu UDP (opartym an datagramach), wysyłamy jeden datagram
                otrzymuję odpowiedź, a następnie połączenie zostaje zakończone
    */
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    /*
    bind() przypisuje adres określony przez address do gniazda określonego przed deskryptor
     serwer_fd. Operacja ta nazywa się "przypisywanie nazwy do gniazda"
    */
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // tutaj ważne żeby lepiej nie używać char *hello bo to oznacza, że tworzymy wskaźnik na jakąś komórkę w pamięci,
    // jeśli wiemy że jest tam char to możemy jego dlugość sprawdzić strlen(), jeśli nie to mamy problem
    // char bufor[__INT32_MAX__]; //jest to rozmiar unsigned int
    // char answear[__INT32_MAX__];
    char bufor[1000];
    string bufor;
    char answear[1000];

    while (1)
    {
        struct sockaddr_in klient;
        socklen_t klientRozmiar = sizeof(klient);

        // odbieramy datagram od klienta
        if (recvfrom(server_fd, bufoor, sizeof(bufor), 0, (struct sockaddr *)&klient, &klientRozmiar) == -1)
        {
            perror("recvfrom() failed");
            exit(EXIT_FAILURE);
        }

        //liczby będę odczytywać wykonując operację modulo 48
        //przykładowo '1' ma wartość 49 czyli 49 mod 48 => 1 itd





        // wysyłamy dane do klienta
        if (sendto(server_fd, answear, sizeof(answear), 0, (struct sockaddr *)&klient, klientRozmiar) == -1)
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