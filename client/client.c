#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

char *server_addr = "127.0.0.1";
int server_port = 8080;

int main(int argc, char const *argv[])
{
    int input = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in in;
    in.sin_family = AF_INET;
    inet_pton(AF_INET, server_addr, &in.sin_addr);
    in.sin_port = htons(server_port);

    if (connect(input, (struct sockaddr *) &in, sizeof(in)) < 0){
        printf("Failed to connect to source");
        exit(1);
    }

    struct sockaddr_in sender_addr;
    sender_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &sender_addr.sin_addr);
    sender_addr.sin_port = htons(9000);

    printf("Stream is available on 127.0.0.1:9000\n");
    fflush(stdout);

    int out = socket(AF_INET, SOCK_DGRAM, 0);

    uint8_t buffer[2048];

    while (1){
        int size = read(input, buffer, 2048);
        // printf("Got %d bytes on input\n", size);

        if (sendto(out, buffer, size, 0, (struct sockaddr *) &sender_addr, sizeof(sender_addr)) == -1){
            fprintf(stderr, "Failed to send bytes on network");
            exit(1);
        }
        // printf("Sent %d bytes on network\n", size);
    }
    return 0;
}
