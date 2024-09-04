#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>  // Include winsock2.h instead of arpa/inet.h

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(SOCKET client_socket) {
    char buffer[BUFFER_SIZE];
    int read_size;

    // Read the client's request
    read_size = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    if (read_size < 0) {
        perror("Failed to read from client");
        closesocket(client_socket);
        return;
    }

    // Null-terminate the received data
    buffer[read_size] = '\0';

    // Print the client's request (optional for debugging)
    printf("Client request:\n%s\n", buffer);

    // Simple HTTP response
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 56\r\n"
        "\r\n"
        "<html><body><h1>Hello, World!</h1></body></html>";

    // Send the response to the client
    send(client_socket, response, strlen(response), 0);

    // Close the client socket
    closesocket(client_socket);
}

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_len;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        perror("Socket creation failed");
        WSACleanup();
        return 1;
    }

    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the specified port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        perror("Socket binding failed");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_socket, 10) == SOCKET_ERROR) {
        perror("Socket listen failed");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Server is listening on port %d...\n", PORT);

    // Main server loop to accept and handle connections
    while (1) {
        client_addr_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == INVALID_SOCKET) {
            perror("Client connection failed");
            closesocket(server_socket);
            WSACleanup();
            return 1;
        }

        // Handle the client request in a separate function
        handle_client(client_socket);
    }

    // Close the server socket (unreachable in this loop)
    closesocket(server_socket);
    WSACleanup();
    return 0;
}

