#include "jnet_utils.h"
#include <asm-generic/socket.h>
#include <pthread.h>

void clean_exit(int signo);

void *log_to_file(void *msg);
pthread_mutex_t logfile_lock;

int udpfd;

const unsigned short PORT = 9999;
#define MAX_MSG_LEN 1024

int main() {
    signal(SIGINT, clean_exit);

    pthread_mutex_init(&logfile_lock, NULL);

    char buffer[MAX_MSG_LEN];
    bzero(buffer, sizeof(buffer));

    int udpfd = safe_socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr = (struct sockaddr_in){
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port = htons(PORT),
        .sin_family = AF_INET,
    };

    safe_bind(udpfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    while (1) {
        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);

        ssize_t n = safe_recvfrom(udpfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&cliaddr, &len);

        buffer[n] = '\0';

        pthread_t tid;

        char *log_buffer = malloc(MAX_MSG_LEN);
        strcpy(log_buffer, buffer);
        printf("%s", log_buffer);

        pthread_create(&tid, NULL, log_to_file, log_buffer);
    }

    pthread_exit(NULL);
}

void clean_exit(int signo) {
    close(udpfd);

    _exit(signo);
}

void *log_to_file(void *buffer) {
    FILE *logfile = fopen("echo.log", "a");
    if (logfile == NULL) {
        perror("fopen");
        exit(1);
    }

    pthread_mutex_lock(&logfile_lock);
    fputs(buffer, logfile);
    pthread_mutex_unlock(&logfile_lock);

    fclose(logfile);
    free(buffer);

    pthread_exit(NULL);
}
