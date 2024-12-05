#include "jnet_utils.h"

void clean_exit(int signo);

const size_t MAX_MSG_LEN = 1024;

int main(int argc, char *argv[]) {
    signal(SIGTERM, clean_exit);
    signal(SIGINT, clean_exit);

    if (argc != 4) {
        printf("Expected command line arguments: `echo_c [ip] [port] [protocol=(tcp, udp)]`.\n");
        exit(1);
    }

    struct in_addr IP;
    if (inet_aton(argv[1], &IP) == 0) {
        printf("Command line argument [ip] format was incorrect.\n");
        exit(1);
    }

    unsigned short PORT;
    if (sscanf(argv[2], "%hu", &PORT) != 1) {
        printf("Command line argument [port] format was incorrect.\n");
        exit(1);
    }

    int protocol;
    if (strcmp(argv[3], "tcp") == 0) {
        protocol = SOCK_STREAM;
    } else if (strcmp(argv[3], "udp") == 0) {
        protocol = SOCK_DGRAM;
    } else {
        printf("Command line argument [protocol] format was incorrect.\n");
        exit(1);
    }

    // socket
    int sock = safe_socket(AF_INET, protocol, 0);
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr = (struct sockaddr_in){
        .sin_addr.s_addr = IP.s_addr,
        .sin_port = htons(PORT),
        .sin_family = AF_INET,
    };

    safe_connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // get input
    char buffer[MAX_MSG_LEN];
    fgets(buffer, MAX_MSG_LEN, stdin);

    if (protocol == SOCK_STREAM) {
        // TCP:
        if (send(sock, buffer, sizeof(buffer), 0) == -1) {
            perror("send");
            exit(1);
        }
    } else {
        // UDP
        safe_sendto(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    }

    close(sock);
    return 0;
}

void clean_exit(int signo) { _exit(signo); }
