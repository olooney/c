#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 7070
#define BUFFER_SIZE 64

// global variable so it's available in the SIGINT signal handler.
int socket_fd;
void sigint_handler(int signal) {
    printf("SIGINT RECEIVED\n");
    if ( socket_fd > 2 ) {
        close(socket_fd);
        printf("CLOSED SOCKET\n");
    }
    exit(0);
}

void die(const char *error_message) {
    perror(error_message);
    exit(1);
}

void mutate(char * str) {
    // change I to O
    for ( char *p = str; *p; p++ ) {
        if ( *p == 'I' ) {
            *p = 'O';
        }
    }
}

void start_server() {
    int new_socket_fd;
    socklen_t client_address_length;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_address, client_address;
    int n_bytes, status;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if ( socket_fd < 0 ) die("Unable to open socket.");

    // initialize server_address
    memset((char *) &server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // bind and start listening on port
    status = bind(
        socket_fd, 
        (struct sockaddr*) &server_address, 
        sizeof(server_address));
    if ( status < 0 ) {
        die("Unable to bind socket.");
    }
    listen(socket_fd, 5);
    client_address_length = sizeof(client_address);
    printf("LISTENING ON PORT %d\n", PORT);

    while ( true ) {
        // accept
        new_socket_fd = accept(
            socket_fd,
            (struct sockaddr *) &client_address, 
            &client_address_length);
        if ( new_socket_fd < 0 ) die("Unable to accept connection.");
        printf("CONNECTED\n");

        while ( true ) {
            // read
            memset(buffer, 0, BUFFER_SIZE);
            n_bytes = read(new_socket_fd, buffer, BUFFER_SIZE-1);
            if ( n_bytes < 0 ) die("failed to read from socket.");
            printf("RECEIVED: %s\n", buffer);


            // connection was closed by the client
            if ( n_bytes == 0 ) {
                printf("CONNECTION CLOSED\n");
                close(new_socket_fd);
                break;
            }

            // do "work"
            mutate(buffer);

            // echo back
            n_bytes = write(new_socket_fd, buffer, strlen(buffer));
            if ( n_bytes < 0 ) die("Unable to write to socket.");
            printf("SENT: %s\n", buffer);
        }

    }

    close(new_socket_fd);
    close(socket_fd);
    die("done serving");



}

void init_signal_handlers() {
    struct sigaction sa;

    // SIG INT
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigint_handler;
    sigaction(SIGINT, &sa, NULL);

    // SIGTERM - no handler, let it die.
}

int main(int argc, char** argv) {
    init_signal_handlers();
    start_server();
    return 0;
}
