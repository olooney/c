#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

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


void set_non_blocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if ( flags == -1 ) die("fcntl get failed");
    flags |= O_NONBLOCK;
    if ( fcntl(fd, F_SETFL, flags) == -1 ) die("fcntl set failed");
}


void set_reuse_addr(int fd) {
    int optval = 1; // Enable the option
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        die("setsockopt(SO_REUSEADDR) failed");
    }
}


void start_server() {
    socklen_t client_address_length;
    struct sockaddr_in server_address, client_address;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if ( socket_fd < 0 ) die("Unable to open socket.");
    set_non_blocking(socket_fd);
    set_reuse_addr(socket_fd);

    // initialize server_address
    memset((char *) &server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // bind and start listening on port
    int status = bind(
        socket_fd, 
        (struct sockaddr*) &server_address, 
        sizeof(server_address));
    if ( status < 0 ) {
        die("Unable to bind socket.");
    }
    listen(socket_fd, 5);
    client_address_length = sizeof(client_address);
    printf("LISTENING ON PORT %d\n", PORT);

    fd_set read_fds, temp_fds;
    FD_ZERO(&read_fds);
    FD_SET(socket_fd, &read_fds);

    char buffer[BUFFER_SIZE];
    int max_fd = socket_fd;

    while ( true ) {
        temp_fds = read_fds;
        status = select(max_fd+1, &temp_fds, NULL, NULL, NULL);
        if ( status < 0 ) die("select error");

        for ( int i=0; i <= max_fd; i++ ) {
            if ( FD_ISSET(i, &temp_fds) ) {
                if ( i == socket_fd ) {
                    int new_socket = accept(
                        socket_fd, 
                        (struct sockaddr*) &client_address, 
                        &client_address_length
                    );
                    if ( new_socket < 0 ) die("accept error");
                    set_non_blocking(new_socket);
                    FD_SET(new_socket, &read_fds);
                    if ( new_socket > max_fd ) max_fd = new_socket;
                    printf("CONNECTED %d\n", i);
                } else {
                    memset(buffer, 0, BUFFER_SIZE);
                    int n_bytes = read(i, buffer, BUFFER_SIZE-1);
                    if ( n_bytes < 0 ) {
                        perror("Failed to read from socket");
                        close(i);
                        FD_CLR(i, &read_fds);
                    } else if ( n_bytes == 0 ) {
                        printf("CONNECTION %d CLOSED\n", i);
                        close(i);
                        FD_CLR(i, &read_fds);
                    } else {
                        // happy path
                        printf("CONNECTION %d RECEIVED: %s\n", i, buffer);
                        mutate(buffer);
                        n_bytes = write(i, buffer, strlen(buffer));
                        if ( n_bytes < 0 ) {
                            perror("Failed to write to socket");
                            close(i);
                        }
                        printf("CONNECTION %d SENT: %s\n", i, buffer);

                    }
                }
            }
        }
    }

}

void init_signal_handlers() {
    struct sigaction sa;

    // SIGINT
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigint_handler;
    sigaction(SIGINT, &sa, NULL);

    // SIGTERM 
    // no handler, let it die.
}

int main(int argc, char** argv) {
    init_signal_handlers();
    start_server();
    return 0;
}
