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
    int socket_des;

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

int main(int argc, char const *argv[])
{
    int port = 2020;

    return 0;
}
