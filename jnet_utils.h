#pragma once

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int max(int x, int y);

int safe_socket(int domain, int type, int protocol);

void safe_bind(int fd, struct sockaddr *addr, socklen_t len);

void safe_listen(int fd, int n);

ssize_t safe_sendto(int fd, const void *buf, size_t n, int flags, const struct sockaddr *addr, socklen_t addr_len);

ssize_t safe_recvfrom(int fd, void *buf, size_t n, int flags, struct sockaddr *addr, socklen_t *addr_len);

void safe_connect(int fd, struct sockaddr *addr, socklen_t addr_len);
