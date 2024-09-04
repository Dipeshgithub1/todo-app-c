TODO App in C
This repository contains a simple HTTP server written in C that responds to client requests with a basic "Hello, World!" HTML page. The server listens on port 8080 and handles HTTP GET requests. This project is primarily for educational purposes to demonstrate how to create a basic HTTP server from scratch in C.

Features:
Handles incoming HTTP GET requests.
Responds with a simple HTML page.
Demonstrates basic socket programming in C using the Winsock API for Windows.
Setup:
Clone the repository.
Compile the C program using GCC with Winsock library:
bash
Copy code
gcc todo.c -o todo -lws2_32
Run the server:
bash
Copy code
.\todo
Open your browser and navigate to http://localhost:8080 to see the server response.
Prerequisites:
Windows environment
GCC compiler
Basic knowledge of C programming and socket programming.
