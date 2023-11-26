#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define MAX_EVENTS 10
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef struct {
    int id;
    int client_fd;
} Client;

int main() 
{
    int server_fd, client_fd, epoll_fd, event_count;
    
    struct sockaddr_in server_addr, client_addr;
    
    struct epoll_event event, events[MAX_EVENTS];
    
    Client clients[MAX_CLIENTS];
    char buffer[BUFFER_SIZE] = "ahmed";
    
    // Create a TCP server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    
    // Bind the socket to the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(9900);
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Failed to bind");
        exit(1);
    }
    
    
    // Listen for incoming connections
    if (listen(server_fd, MAX_CLIENTS) == -1) {
        perror("Failed to listen");
        exit(1);
    }
    

    // Create an epoll instance
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("Failed to create epoll");
        exit(1);
    }
    

    // Add the server socket to the epoll instance
    event.events = EPOLLIN;
    event.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        perror("Failed to add server socket to epoll");
        exit(1);
    }
    

    printf("Server started. Listening for connections...\n");
    
    while (1) {
        event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (event_count == -1) {
            perror("Failed to wait for events");
            exit(1);
        }
        
        for (int i = 0; i < event_count; i++) 
        {
            int fd = events[i].data.fd;
            
            if (fd == server_fd) 
            {
                // New client connection
                socklen_t client_len = sizeof(client_addr);
                client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
                if (client_fd == -1) {
                    perror("Failed to accept client connection");
                    exit(1);
                }
                
                // Add client socket to epoll instance
                event.events = EPOLLIN | EPOLLET;
                event.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1) {
                    perror("Failed to add client socket to epoll");
                    exit(1);
                }
                
                // Generate a unique ID for the client
                int client_id = i + 1;
                clients[i].id = client_id;
                clients[i].client_fd = client_fd;
                
                // Send the client ID back to the client
                char id_message[BUFFER_SIZE];
                snprintf(id_message, BUFFER_SIZE, "Your ID is %d\n", client_id);
                send(client_fd, id_message, strlen(id_message), 0);
                
                printf("New client connected. ID: %d\n", client_id);
            }
            else {
                // Data from an existing client
                client_fd = fd;
                int client_id = -1;
                
                // Find client ID based on client socket
                for (int j = 0; j < MAX_CLIENTS; j++) {
                    if (clients[j].client_fd == client_fd) {
                        client_id = clients[j].id;
                        break;
                    }
                }
                
                // Receive message from the client
                int bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
                if (bytes_received <= 0)
                {
                    // Client disconnected
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
                    close(client_fd);
                    
                    printf("Client %d disconnected\n", client_id);
                }
                else {
                    // Send the message back to the client with the client's ID
                    //snprintf(buffer + bytes_received, BUFFER_SIZE - bytes_received, " Client %d", client_id);
                    send(client_fd, buffer, strlen(buffer), 0);
                }
            }
        }
    }
    
    // Cleanup
    close(server_fd);
    close(epoll_fd);
    
    return 0;
}