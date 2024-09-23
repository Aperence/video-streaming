#ifndef transport_h
#define transport_h

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>

typedef struct stream_server{
    int socket;
    int socket_udp;
    int input_socket;
    struct sockaddr_storage addr_udp;
    socklen_t addr_udp_len;
} stream_server_t;

stream_server_t *stream_server_new(char *ip_server, uint16_t port);
void stream_server_run(stream_server_t *server);

#endif /* transport_h */
