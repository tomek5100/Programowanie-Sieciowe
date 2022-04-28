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

bool drukowalne(const char *buf, int size)
{
    // funkcja zwraca false gdy dane nie sa cyframi
    // albo sa puste

    // zmienna do przechowywania liczby cyfr
    int number = 0;

    // pętla przez cały bufor
    for (int i = 0; i < size; i++)
    {
        // jeśli nie cyfra
        if (*buf < 48 || *buf > 57)
        {
            // jeśli spacja, \n lub \r
            if (*(buf) == 13 || *buf == 10 || *buf == 32)
            {
                buf++;
                if ((*buf) == 32)
                {
                    // dwie spacje obok siebie
                    return false;
                }
                continue;
            }
            return false;
        }
        buf++;
        number++;
    }

    // jesli nie pojawiła się liczba to znaczy że same spacje
    if (number == 0)
    {
        return false;
    }

    return true;
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

void sumator(int client_socket)
{
    // funkcja realizuje odczytywanie i odsylanie danych do klienta

    int bufor_size = 1024;
    char bufor[bufor_size];
    memset(bufor, 0, sizeof(bufor));
    char odpowiedz[22];
    memset(odpowiedz, 0, sizeof(odpowiedz));

    while (1)
    {
        bool overflow = false;
        bool not_number = false;
        bool is_empty = false;
        bool zero_byte = false;
        bool end_query = false;
        bool end_line_err = false;
        int bytes_read = 0;

        if ((bytes_read = read(client_socket, bufor, sizeof(bufor))) == -1)
        {
            perror("Read error");
            break;
        }

        int suma = 0;
        char odczytana[22];
        memset(odczytana, 0, sizeof(odczytana));
        int i = 0;
        const char *bufor_pom = bufor;

        if (bytes_read == 0)
        {
            is_empty = true;
            break;
        }
        else if (drukowalne(bufor, bytes_read) == false)
        {
            printf("Otrzymane znaki nie sa cyframi\n");
            not_number = true;
        }

        if (bytes_read > 0)
        {
            printf("odczytano: %s\n", bufor);
            do
            {
                if ((*bufor_pom == 32) && i == 0)
                {
                    // pierwszy znak to spacja
                    not_number = true;
                }
                if ((*bufor_pom == ' ') || (*bufor_pom == 10))
                {
                    long cast = strtol(odczytana, 0, 10);
                    if ((cast == LONG_MAX) || (cast == LONG_MIN))
                    {
                        printf("Przekroczono zakres long podczas konwersji\n");
                        overflow = true;
                    }

                    /*
                    de facto drugi warunek lepiej sprawdza overflow
                    bo odejmuje max zakres inta od przekonwertowanej liczby
                    natomiast funkcja atoi(odczytana) moze zwrocic bledny wynik
                    */
                    if ((suma > (INT_MAX - atoi(odczytana))) || (cast - INT_MAX > 0))
                    {
                        printf("OVERFLOW\n");
                        overflow = true;
                    }
                    suma += atoi(odczytana);
                    memset(odczytana, 0, sizeof(odczytana));
                    i = 0;
                }

                if (*bufor_pom == '\0')
                {
                    printf("Odczytano bajt o wartosci 0\n");
                    zero_byte = true;
                }
                if (*bufor_pom == 13)
                {
                    // natrafilismy na 1 znak konca zapytania
                    end_query = true;
                    bufor_pom++;
                    continue;
                }
                else if (*bufor_pom == 10)
                {
                    // koniec zapytania, mozemy odeslac odpowiedz
                    if (!end_query)
                    {
                        // jesli poprzednim znakiem nie bylo \r to blad
                        end_line_err = true;
                    }
                    else
                    {
                        // tu wysylamy odpowiedz
                        int dlugosc_odpowiedzi = 0;
                        
                        if ((overflow == true) || (not_number == true) || (is_empty == true) || (zero_byte == true) || (end_line_err == true))
                        {
                            dlugosc_odpowiedzi = sprintf(odpowiedz, "ERROR\r\n");
                        }
                        else
                        {
                            dlugosc_odpowiedzi = sprintf(odpowiedz, "%d\r\n", suma);
                        }

                        if (write(client_socket, odpowiedz, dlugosc_odpowiedzi) == -1)
                        {
                            perror("Write() error");
                            exit(EXIT_FAILURE);
                        }
                    }
                    // jestesmy gotowi do odczytywania kolejnych danych
                    end_query = false;
                    bufor_pom++;
                    continue;
                }
                // kolejnym znakiem nie bylo \n wiec nie byl to koniec zapytania
                end_query = false;

                odczytana[i] = *bufor_pom;
                i++;
                bufor_pom++;
            } while (*bufor_pom != 0);
        }
    }
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
        else
        {
            char connection_info[INET_ADDRSTRLEN];
            if (inet_ntop(AF_INET, &ClientAddress.sin_addr, connection_info, sizeof(connection_info)) == NULL)
            {
                perror("Inet_ntop error");
                strcpy(connection_info, "???");
            }
            std::cout << "Nowe połączenie " << connection_info << ":" << (unsigned int)ntohs(ClientAddress.sin_port) << std::endl;
        }

        // ustawianie timeoutu serwera
        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        if (setsockopt(client_des, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1)
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        // forkujemy
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("Fork error");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) // proces child
        {

            // zamykamy deskryptor serwera
            if (close(server_des) == -1)
            {
                perror("Close server socket error");
                exit(EXIT_FAILURE);
            }

            // odczytujemy dane od klienta
            sumator(client_des);

            // nastepnie zamykamy deskryptor proces child
            if (close(client_des) == -1)
            {
                perror("Close client socket error");
                exit(EXIT_FAILURE);
            }

            // tutaj proces jeszcze dziala, nalezy go zakonczyc
            exit(0);
        }
        else
        {
            // instrukcje wykonywane w parent-process
            ;
        }
        // zamykamy deskryptor klienta w procesie-rodzicu oraz na wypadek bledu fork()
        if ((close(client_des)) == -1)
        {
            perror("Close client socket error from parent");
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char const *argv[])
{
    int port = 2020;
    int server_des = -1;

    // tworze gniazdko
    if ((server_des = listening_socket(port)) == -1)
    {
        perror("socket create error");
        exit(EXIT_FAILURE);
    }

    // glowna petla serwera tworzaca nowe procesy dla klientow
    forking_loop(server_des);

    // po petli zamykam deskryptor otwartego gniazdka serwera!
    if (close(server_des) == -1)
    {
        perror("Close error");
        exit(EXIT_FAILURE);
    }

    return 0;
}
