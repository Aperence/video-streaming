#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>

typedef struct client{
    int socket;
    struct sockaddr_storage addr;
    socklen_t len;
} client_t;

typedef struct node{
    client_t *client;
    struct node *next;
} node_t;

int main(int argc, char const *argv[])
{
    int input = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in in;
    in.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &in.sin_addr);
    in.sin_port = htons(8000);

    if (bind(input, (struct sockaddr *) &in, sizeof(in)) < 0){
        printf("Failed to connect to source");
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);
    addr.sin_port = htons(8080);

    int server = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(server, F_SETFL, O_NONBLOCK);
    int val = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if (bind(server, (struct sockaddr *) &addr, sizeof(addr)) < 0){
        fprintf(stderr, "Failed to bind");
        exit(1);
    }
    listen(server, 3);

    uint8_t buffer[2048];
    uint8_t peer_buf[2048];

    struct sockaddr_storage client;
    socklen_t client_len;

    node_t *clients = NULL;

    while (1){
        int client = accept(server, (struct sockaddr *) &client, &client_len);
        if (client != -1){
            printf("New client\n");
            node_t *new_client = malloc(sizeof(node_t));
            new_client->client = malloc(sizeof(client_t));
            memcpy(&new_client->client->addr, &client, client_len);
            new_client->client->len = client_len;
            new_client->client->socket = client;
            new_client->next = clients;
            clients = new_client;
        }

        int size = read(input, buffer, 2048);

        node_t *prev = NULL;
        node_t *curr = clients;
        while (curr){
            if (send(curr->client->socket, buffer, size, MSG_NOSIGNAL) < 0){
                printf("Client left\n");
                node_t *temp = curr->next;
                if (!prev)
                    clients = temp;
                else
                    prev->next = temp;
                free(curr->client);
                free(curr);
                curr = temp;
            }else{
                prev = curr;
                curr = curr->next;
            }
        }
    }
    return 0;
}
