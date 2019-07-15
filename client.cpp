#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

int main(int argc, const char * argv[])
{
    int sockfd;
    struct addrinfo hints, *res;
    const char * hostname = argv[1];
    const char * port = argv[2];
    char buf[1024];


    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;

    if( (getaddrinfo(hostname, port, &hints, &res)) < 0) {
        perror("getaddrinfo");
        exit(1);
    }

    if( (sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0 ) {
        perror("socket");
        exit(1);
    }

    if(::connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        close(sockfd);
        perror("connect");
        exit(1);
    }

    freeaddrinfo(res);

    int rsize;
    while(1) {
        std::cout << " > ";
        std::cin >> buf;
        if(!strncmp(buf, "end", 3)) {
            break;
        }
        if((send(sockfd, buf, strlen(buf), 0)) < 0) {
            perror("send error");
            exit(1);
        }
        printf("send: %s\n", buf);

        rsize = recv(sockfd, buf, sizeof(buf), 0);

        if(rsize == 0) {
            break;
        } else if(rsize == -1) {
            perror("receive error\n");
        } else {
            printf("receive: %s\n", buf);
        }
    }

    close(sockfd);

    return 0;
}
