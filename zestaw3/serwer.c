//
// Created by Domi1337 on 3/9/2022.
//
#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
unsigned int sumowanie(const char* liczby) {
    unsigned int suma=0;
    char sumapom[11];
    int i=0;
    while (*liczby != 0) {
        if(*liczby==13||*liczby==10){
            liczby++;
            continue;
        }
        if (*liczby == ' ') {
            suma += atoi(sumapom);
            memset(sumapom,0,sizeof(sumapom));
            i=0;
        }
        sumapom[i] = *liczby;
        i++;
        liczby++;
    }
    if(suma>UINT_MAX-atoi(sumapom)){
        return -1;
    }
    suma += atoi(sumapom);
    memset(sumapom,0,sizeof(sumapom));
    return(suma);
}
bool drukowalne(const char * buf){
    while(1){
        if(*buf==0){
            return true;
        }
        if(*buf<48||*buf>57){
            if(*(buf)==13||*buf==10||*buf==32) {
                buf++;
                continue;
            }
            return false;
        }
        buf++;
    }
}
int main(int argc, char const* argv[]){
    if(argc!=2){
        perror("usage: ./serwer.x [port]");
        exit(1);
    }

    int servSockD = socket(AF_INET, SOCK_DGRAM, 0);
    if(servSockD<0){
        perror("socket create error");
        exit(4);
    }

    char buffer[65507];

    struct sockaddr_in servAddr;

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(atoi(argv[1]));
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(servSockD, (struct sockaddr*)&servAddr,sizeof(servAddr))!=0){
        perror("socket bind failed...\n");
        exit(1);
    }
    int len = sizeof( servAddr );
    while(1) {
        struct sockaddr_in client = { };
        memset( buffer, 0, sizeof( buffer ) );
        printf( "Waiting for connection...\n" );
        int ostatni=recvfrom( servSockD, buffer, sizeof( buffer )-1, 0, (struct sockaddr*)&client, &len);
        if (ostatni==-1){
            perror( "recvfrom() ERROR" );
            exit( 2 );
        }
        if(buffer[ostatni-1]!=0){
            buffer[ostatni]=0;
        }

        if(drukowalne(buffer)){
            printf("received: %s\n",buffer);
        }
        else{
            printf("znaki nie sa drukowalne");
            exit(1);
        }
        char wynik[11]="ERROR";
        unsigned int wynikpom=sumowanie(buffer);
        if(wynikpom!=-1){
            memset(wynik,0,sizeof(wynik));
            sprintf(wynik, "%d", wynikpom);
        }
        if(sendto(servSockD,wynik, strlen(wynik), 0,(struct sockaddr*)&client, len)<0){
            perror( "sendto() ERROR" );
            exit( 3 );
        }
    }
    close(servSockD);
    return 0;
}