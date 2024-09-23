#include "transport.h"
#include "errno.h"

stream_server_t *stream_server_new(char *ip_server, uint16_t port)
{
    int input = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in in;
    in.sin_family = AF_INET;
    inet_pton(AF_INET, ip_server, &in.sin_addr);
    in.sin_port = htons(port);

    if (connect(input, (struct sockaddr *) &in, sizeof(in)) < 0){
        printf("Failed to connect to source");
        return NULL;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    addr.sin_port = htons(9000);

    struct sockaddr_in addr_udp;
    addr_udp.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &addr_udp.sin_addr);
    addr_udp.sin_port = htons(9001);

    int server = socket(AF_INET, SOCK_STREAM, 0);
    int val = 1;
    if (setsockopt(server, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val)) < 0){
        fprintf(stderr, "Failed to set socket option");
        return NULL;
    }
    if (bind(server, (struct sockaddr *) &addr, sizeof(addr)) < 0){
        fprintf(stderr, "Failed to bind tcp server");
        //return NULL;
    }
    if (listen(server, 2) < 0){
        fprintf(stderr, "Failed to listen tcp server");
        return NULL;
    }
    
    int server_udp = socket(AF_INET, SOCK_DGRAM, 0);

    printf("Local server listening on 127.0.0.1:9000\n");
    printf("Local UDP server sending on 127.0.0.1:9001\n");

    stream_server_t *stream_server = malloc(sizeof(stream_server_t));
    stream_server->socket = server;
    stream_server->input_socket = input;
    stream_server->socket_udp = server_udp;
    memcpy(&stream_server->addr_udp, &addr_udp, sizeof(addr_udp));
    stream_server->addr_udp_len = sizeof(addr_udp);
    
    return stream_server;
}

void stream_server_run(stream_server_t *server){
    uint8_t buffer[2048];
    
    struct sockaddr_storage client_addr;
    socklen_t client_len;
    
    int client = accept(server->socket, (struct sockaddr *) &client_addr, &client_len);

    ssize_t err;

    while (1){
        ssize_t size = read(server->input_socket, buffer, 2048);

        err = sendto(server->socket_udp, buffer, size, 0, (struct sockaddr *) &server->addr_udp, server->addr_udp_len);
        if (err < 0){
            fprintf(stderr, "Failed to send on udp socket");
        }

        if (send(client, buffer, size, 0) < 0){
            break;
        }
    }
}
