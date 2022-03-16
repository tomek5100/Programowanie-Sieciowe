//serwer
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>

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

    int server_fd, new_socket;

    /*
    AF_INET - obsługa protokołu IPv4
    SOCK_STREM - odnosi się do protokołu TCP, połączenie jest nawiązywane i obie strony prowadzą
                rozmowę dopóki połączenie nie zostanie przerwane przez jedną ze stron
    */
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
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

    /*
    listen() określa, że gniazdo będzie używane do przyjmowania przychodzących połączeń za pomocą
    funkcji accept()
    drugi argument określa maksymalną długość, do któej kolejka oczekujących połączeń może wzrosnąć
    */
    if (listen(server_fd, 3) == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    char *hello = "Hello, world!\r\n";

    while (1)
    {
        /*
        accept() obsługuje pierwsze połączenie w kolejce, tworzy nowe gniazdo i zwraca jego deskryptor
        */
        if ((new_socket = accept(server_fd, NULL, NULL) == -1))
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        write(new_socket, hello, strlen(hello));
        printf("Hello message sent\n");

        if (close(new_socket) == -1)
        {
            perror("close");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}