#include "jnet_utils.h"
#include <time.h>

void send_log(char *buffer, struct sockaddr_in cliaddr);
void clean_exit(int signo);

#define MAX_MSG_LEN 1024
#define MAX_LOG_LEN (MAX_MSG_LEN + 64)

int tcpfd, logfd, udpfd;

int main(int argc, char *argv[]) {
    signal(SIGTERM, clean_exit);
    signal(SIGINT, clean_exit);
    signal(SIGCHLD, SIG_IGN);

    if (argc != 2) {
        printf("Expected command line argument: `echo_s <port>`.\n");
        exit(1);
    }

    unsigned short PORT;
    if (sscanf(argv[1], "%hu", &PORT) != 1) {
        printf("Command line argument `echo_s <port>` format was incorrect.\n");
        exit(1);
    }

    char buffer[MAX_MSG_LEN];

    // socket
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr = (struct sockaddr_in){
        .sin_family = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port = htons(PORT),
    };

    // TCP and UDP socket
    tcpfd = safe_socket(AF_INET, SOCK_STREAM, 0);
    safe_bind(tcpfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    safe_listen(tcpfd, 10);

    udpfd = safe_socket(AF_INET, SOCK_DGRAM, 0);
    safe_bind(udpfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    logfd = safe_socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in logaddr;
    bzero(&logaddr, sizeof(logaddr));
    logaddr = (struct sockaddr_in){
        .sin_family = AF_INET,
        .sin_addr.s_addr = inet_addr("127.0.0.1"),
        .sin_port = htons(9999),
    };
    safe_connect(logfd, (struct sockaddr *)&logaddr, sizeof(logaddr));

    fd_set rset;
    FD_ZERO(&rset);

    int maxfdp1 = max(tcpfd, udpfd) + 1;

    while (1) {
        bzero(buffer, sizeof(buffer));

        FD_SET(tcpfd, &rset);
        FD_SET(udpfd, &rset);

        socklen_t len;
        struct sockaddr_in cliaddr;

        select(maxfdp1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(tcpfd, &rset)) {
            len = sizeof(cliaddr);
            int connfd = accept(tcpfd, (struct sockaddr *)&cliaddr, &len);
            pid_t childpid;
            if ((childpid = fork()) == 0) {
                close(tcpfd);
                bzero(buffer, sizeof(buffer));
                read(connfd, buffer, sizeof(buffer));
                write(connfd, buffer, sizeof(buffer));

                close(connfd);
                send_log(buffer, cliaddr);
                clean_exit(0);
            }
            close(connfd);
        }

        else if (FD_ISSET(udpfd, &rset)) {
            pid_t childpid;
            if ((childpid = fork()) == 0) {
                len = sizeof(cliaddr);
                bzero(buffer, sizeof(buffer));
                safe_recvfrom(udpfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&cliaddr, &len);
                safe_sendto(udpfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
                send_log(buffer, cliaddr);
                clean_exit(0);
            }
        }
    }

    clean_exit(0);
}

void clean_exit(int signo) {
    close(tcpfd);
    close(udpfd);
    close(logfd);

    _exit(signo);
}

void send_log(char *buffer, struct sockaddr_in cliaddr) {
    buffer[strlen(buffer) - 1] = '\0';

    char log_msg[MAX_LOG_LEN];

    time_t timestamp;
    time(&timestamp);

    char timestamp_str[20];
    strftime(timestamp_str, 20, "%Y-%m-%d %H:%M:%S", localtime(&timestamp));

    snprintf(log_msg, MAX_LOG_LEN, "%s \"%s\" was received from %s\n", timestamp_str, buffer,
             inet_ntoa(cliaddr.sin_addr));

    printf("%s", log_msg);

    write(logfd, log_msg, strlen(log_msg));
}
