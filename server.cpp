#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <fstream>
#include "Base64.h"
#include "FileTransfer.h"
#include "Response.h"
#include <iomanip>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib") // Some-times it will give Undefined Refference Errors This is to prevent that

#define PORT 8080
const std::string file_path = "G:\\## test\\Pf-ProjectFinal\\Received Files";


void SreceiveAndProcessResponse(SOCKET socket) {
    char buffer[1024] = { 0 };
    int valread = recv(socket, buffer, sizeof(buffer), 0);
    if (valread <= 0) {
        std::cerr << "Error receiving response from the server" << std::endl;
        return;
    }
    processResponse(socket, buffer);
}

void sprintMessageWithDelay(const std::string& message, int delay) {
    for (char c : message) {
        std::cout << c;
        Sleep(delay); // Delay between characters
    }
    std::cout << std::endl;
}



void serverMenu(SOCKET socket) {
    int choice;
    std::string message;
    char buffer[1024] = { 0 };
    int valread;  // Move the declaration here
    std::string decryptedMessage;
    do {
        std::string menutr = "Server Menu:\n1. Send message\n2. Send file\n3. Exit\nEnter your choice: ";
        sprintMessageWithDelay(menutr, 80);

        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1:
                std::cout << "Enter message to send: "<< std::endl;
                std::getline(std::cin, message);
                message = base64_encode(message);
                send(socket, message.c_str(), message.size(), 0);
                std::cout << "Encrypted message has been sent" << std::endl;
                SreceiveAndProcessResponse(socket);
                break;


            case 2: {
                std::string filePath;
                std::cout << "Enter file path to send: ";
                std::getline(std::cin, filePath);
                sendFile(socket, filePath);
                break;
            }
            case 3:
                send(socket, "exit", 4, 0);
                std::cout << "Exit command sent, closing connection" << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    } while (choice != 3);
}





void startServer() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    SOCKET server_fd, new_socket;
    sockaddr_in address;
    int addrlen = sizeof(address);

    char buffer[1024] = { 0 };

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Socket created" << std::endl;

    // Allow address reuse
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&opt), sizeof(opt)) ==
        SOCKET_ERROR) {
        std::cerr << "setsockopt failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Socket options set" << std::endl;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Binding the socket to the specified port
    if (bind(server_fd, reinterpret_cast<struct sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Binding failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Binding successful" << std::endl;

    if (listen(server_fd, 3) == SOCKET_ERROR) {
        std::cerr << "Listening failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Listening for incoming connections..." << std::endl;

    while (true) {
        if ((new_socket = accept(server_fd, reinterpret_cast<struct sockaddr*>(&address), &addrlen)) ==
            INVALID_SOCKET) {
            std::cerr << "Accepting connection failed" << std::endl;
            continue;
        }
        std::cout << "Connection accepted" << std::endl;

        serverMenu(new_socket);
        processResponse(new_socket,buffer);

        closesocket(new_socket);
        std::cout << "Connection closed" << std::endl;
        std::cout << "Listening for incoming connections..." << std::endl;
    }

    closesocket(server_fd);
    WSACleanup();

}