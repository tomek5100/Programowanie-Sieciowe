//@Tomasz Rozycki
//  serwer TCP
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <signal.h>

int error_close_socket(int s)
{
    // awaryjne zamkniecie na wypadek bledu
    close(s);
    return -1;
}

int listening_socket(in_port_t port)
{
    // TCP IPv4
    int socket_des = -1;

    if ((socket_des = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket failed");
        return -1;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_des, (struct sockaddr *)&address, sizeof(address)) == -1)
    {
        perror("bind failed");
        socket_des = error_close_socket(socket_des);
    }

    if (listen(socket_des, 10) == -1)
    {
        perror("listen");
        socket_des = error_close_socket(socket_des);
    }

    // zwracam deskryptor socketu albo -1 jesli blad
    return socket_des;
}

int read_client(int client_socket)
{
    // funkcja realizuje odsylanie danych do klienta
    int bytes_read = 0;

    return bytes_read;
}

void forking_loop(int server_des)
{
    // funkcja tworzy osobny proces potomny dla kazdego klienta

    // dajemy znać jądru że nie będziemy prosić o statusy dzieci; od razu po
    // zakończeniu działania można usunąć ich wpisy w tablicy procesów
    if (signal(SIGCHLD, SIG_IGN) == SIG_ERR)
    {
        return;
    }

    while (true)
    {
        struct sockaddr_in ClientAddress;
        socklen_t ClientAddress_len = sizeof(ClientAddress);
        int client_des = accept(server_des, (struct sockaddr *)&ClientAddress, &ClientAddress_len);
        if (client_des == -1)
        {
            perror("Accept error");
            return;
        }

        // forkujemy
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("Fork error");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // proces child

            // zamykamy deskryptor serwera
            if (close(server_des) == -1)
            {
                perror("Close server socket error");
                exit(EXIT_FAILURE);
            }

            // odczytujemy dane od klienta
            while (read_client(client_des) > 0)
            {
                ;
            }

            // nastepnie zamykamy deskryptor proces child
            if (close(client_des) == -1)
            {
                perror("Close client socket error");
                exit(EXIT_FAILURE);
            }

            //tutaj proces jeszcze dziala, nalezy go zakonczyc
            exit(0);
        }
        else
        {
            // instrukcje wykonywane w parent-process
            ;
        }
        // zamykamy deskryptor klienta w procesie-rodzicu oraz na wypadek bledu fork()
    }
}

int main(int argc, char const *argv[])
{
    int port = 2020;
    int server_des = -1;

    // tworze gniazdko
    if (server_des = listening_socket(port) == -1)
    {
        perror("socket create error");
        exit(EXIT_FAILURE);
    }

    // tutaj ma byc glowna petla serwera

    forking_loop(server_des);

    // po petli zamykam deskryptor otwartego gniazdka serwera!
    if (close(server_des) == -1)
    {
        perror("Close error");
        exit(EXIT_FAILURE);
    }

    return 0;
}
