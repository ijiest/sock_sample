#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char const* argv[])
{
    int sockfd, client_sockfd;
    struct sockaddr_in addr;
    struct sockaddr_in client;
    socklen_t len = sizeof(struct sockaddr_in);
    int port = atoi(argv[1]);
    char buf[1024];

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error\n");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind error\n");
        exit(1);
    }

    if(listen(sockfd, SOMAXCONN) < 0) {
        perror("listen error\n");
        exit(1);
    }

    if((client_sockfd = accept(sockfd, (struct sockaddr *)&client, &len)) < 0) {
        perror("accept error\n");
        exit(1);
    }
    printf("accept connection from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    int rsize;
    while(1) {
        rsize = recv(client_sockfd, buf, sizeof(buf), 0);

        if(rsize == 0) {
            break;
        } else if(rsize == -1) {
            perror("receive error\n");
        } else {
            printf("receive: %s\n", buf);
            // echo same massage
            if((send(client_sockfd, buf, strlen(buf), 0)) < 0) {
                perror("send error");
            }
            printf("send: %s\n", buf);
            memset(buf, 0, sizeof(buf));
        }
    }

    close(client_sockfd);
    close(sockfd);

    return 0;
}
