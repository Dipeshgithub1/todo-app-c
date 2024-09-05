#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(SOCKET client_socket);

int main() {
    WSADATA wsaData;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_size = sizeof(client_addr);
    HANDLE thread;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    // Create a socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    // Setup the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port %d\n", PORT);

    // Accept incoming connections and handle them using threads
    while (1) {
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size)) == INVALID_SOCKET) {
            printf("Accept failed.\n");
            closesocket(server_socket);
            WSACleanup();
            return 1;
        }

        // Create a new thread to handle the client
        thread = (HANDLE)_beginthreadex(NULL, 0, (unsigned int (__stdcall *)(void *))handle_client, (void *)client_socket, 0, NULL);
        if (thread == NULL) {
            printf("Thread creation failed.\n");
            closesocket(client_socket);
        } else {
            CloseHandle(thread);
        }
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}

void handle_client(SOCKET client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    char response[BUFFER_SIZE] = {0};
    int bytes_received;  // Declare once
    const char *html_content = "<!DOCTYPE html>\n"
                               "<html lang=\"en\">\n"
                               "<head>\n"
                               "    <meta charset=\"UTF-8\">\n"
                               "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                               "    <title>Your Name - Portfolio</title>\n"
                               "    <style>\n"
                               "        body {\n"
                               "            font-family: Arial, sans-serif;\n"
                               "            margin: 0;\n"
                               "            padding: 0;\n"
                               "            color: #333;\n"
                               "        }\n"
                               "        header {\n"
                               "            background-color: #4CAF50;\n"
                               "            color: white;\n"
                               "            padding: 1rem;\n"
                               "            text-align: center;\n"
                               "        }\n"
                               "        section {\n"
                               "            padding: 2rem;\n"
                               "            margin: 1rem;\n"
                               "        }\n"
                               "        .container {\n"
                               "            max-width: 800px;\n"
                               "            margin: 0 auto;\n"
                               "        }\n"
                               "        h2 {\n"
                               "            color: #4CAF50;\n"
                               "        }\n"
                               "        footer {\n"
                               "            background-color: #f1f1f1;\n"
                               "            text-align: center;\n"
                               "            padding: 1rem;\n"
                               "            position: fixed;\n"
                               "            width: 100%;\n"
                               "            bottom: 0;\n"
                               "        }\n"
                               "    </style>\n"
                               "</head>\n"
                               "<body>\n"
                               "    <header>\n"
                               "        <h1>Your Name</h1>\n"
                               "        <p>Welcome to my portfolio</p>\n"
                               "    </header>\n"
                               "    <section>\n"
                               "        <div class=\"container\">\n"
                               "            <h2>About Me</h2>\n"
                               "            <p>Hello! I'm [Your Name], a [Your Profession/Title] with expertise in [Your Skills]. I have a passion for [Your Passion/Field] and enjoy working on challenging projects.</p>\n"
                               "            <h2>Projects</h2>\n"
                               "            <div class=\"project\">\n"
                               "                <h3>Project 1</h3>\n"
                               "                <p>Description of your project. Include what technologies you used, what you accomplished, and a link if applicable.</p>\n"
                               "            </div>\n"
                               "            <div class=\"project\">\n"
                               "                <h3>Project 2</h3>\n"
                               "                <p>Description of your project. Include what technologies you used, what you accomplished, and a link if applicable.</p>\n"
                               "            </div>\n"
                               "            <h2>Contact</h2>\n"
                               "            <p>You can reach me at: <a href=\"mailto:your.email@example.com\">your.email@example.com</a></p>\n"
                               "        </div>\n"
                               "    </section>\n"
                               "    <footer>\n"
                               "        <p>&copy; 2024 Your Name. All rights reserved.</p>\n"
                               "    </footer>\n"
                               "</body>\n"
                               "</html>";

    // Receive client request
    bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);

    if (bytes_received > 0) {
        buffer[bytes_received] = '\0'; // Null-terminate the string
        printf("Request received:\n%s\n", buffer);

        // Handle GET request
        if (strncmp(buffer, "GET ", 4) == 0) {
            // Serve the HTML content
            snprintf(response, sizeof(response), 
                     "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/html\r\n"
                     "Content-Length: %ld\r\n"
                     "\r\n%s", 
                     strlen(html_content), html_content);
        } else {
            // Unsupported HTTP method
            snprintf(response, sizeof(response), 
                     "HTTP/1.1 404 Not Found\r\n"
                     "Content-Type: text/plain\r\n"
                     "Content-Length: 13\r\n"
                     "\r\n404 Not Found");
        }

        // Send response to client
        send(client_socket, response, strlen(response), 0);
    }

    // Clean up and close connection
    closesocket(client_socket);
}
