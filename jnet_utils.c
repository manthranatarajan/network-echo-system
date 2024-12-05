#include "jnet_utils.h"
#include <asm-generic/socket.h>
#include <stdio.h>
#include <sys/socket.h>

#pragma GCC diagnostic ignored "-Wformat"

int max(int x, int y) { return x > y ? x : y; }

int safe_socket(int domain, int type, int protocol) {
    int sock = socket(domain, type, protocol);

    if (sock == -1) {
        fprintf(stderr, "socket error: [%s], arguments: socket(domain: %d, type: %d, protocol: %d)\n", strerror(errno),
                domain, type, protocol);
        exit(1);
    }

    return sock;
}

void safe_bind(int fd, struct sockaddr *addr, socklen_t len) {
    if (bind(fd, (struct sockaddr *)addr, len) == -1) {
        fprintf(stderr, "bind error: [%s], arguments: bind(fd: %d, addr: %p, len: %u)\n", strerror(errno), fd, addr,
                len);
        exit(2);
    }
}

void safe_listen(int fd, int n) {
    if (listen(fd, n) == -1) {
        fprintf(stderr, "listen error: [%s], arguments: listen(fd: %d, n: %d)\n", strerror(errno), fd, n);
        exit(3);
    }
}

ssize_t safe_sendto(int fd, const void *buf, size_t n, int flags, const struct sockaddr *addr, socklen_t addr_len) {
    int bytes_sent = sendto(fd, buf, n, flags, addr, addr_len);

    if (bytes_sent == -1) {
        fprintf(stderr,
                "sendto error: [%s], arguments: sendto(fd: %d, buf: %p, n: %zu, flags: %d, addr: %p, addr_len: %u)\n",
                strerror(errno), fd, buf, n, flags, addr, addr_len);
        exit(3);
    }

    return bytes_sent;
}

ssize_t safe_recvfrom(int fd, void *buf, size_t n, int flags, struct sockaddr *addr, socklen_t *addr_len) {
    int bytes_read = recvfrom(fd, buf, n, flags, addr, addr_len);

    if (bytes_read == -1) {
        fprintf(
            stderr,
            "recvfrom error: [%s], arguments: recvfrom(fd: %d, buf: %p, n: %zu, flags: %d, addr: %p, addr_len: %p)\n",
            strerror(errno), fd, buf, n, flags, addr, addr_len);
        exit(3);
    }

    return bytes_read;
}

void safe_connect(int fd, struct sockaddr *addr, socklen_t addr_len) {
    if (connect(fd, addr, addr_len) == -1) {
        fprintf(stderr, "connect error: [%s], arguments: connect(fd: %d, addr: %p, addr_len: %u)\n", strerror(errno),
                fd, addr, addr_len);
        exit(4);
    }
}
