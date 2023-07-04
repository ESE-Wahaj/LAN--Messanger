#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include "Base64.h"
#include "FileTransfer.h"
#include "Response.h"
#define PORT 8080
const std::string file_path = "G:\\## test\\Pf-ProjectFinal\\Received Files";


void cprintMessageWithDelay(const std::string& message, int delay) {
    for (char c : message) {
        std::cout << c;
        Sleep(delay); // Delay between characters
    }
    std::cout << std::endl;
}


void CreceiveAndProcessResponse(SOCKET socket) {
    char buffer[1024] = { 0 };
    int valread = recv(socket, buffer, sizeof(buffer), 0);
    if (valread <= 0) {

        std::string ServerDisconnect = "Server is Shut-Downed!";
        cprintMessageWithDelay(ServerDisconnect, 80);
        return;
    }
    processResponse(socket, buffer);
}


void clientMenu(SOCKET socket) {
    int choice;
    std::string message;
    char buffer[1024] = { 0 };

    std::string decryptedMessage; // Move the declaration here

    do {
        std::string menutc = "Server Menu:\n1. Send message\n2. Send file\n3. Exit\nEnter your choice: ";
        cprintMessageWithDelay(menutc, 80);
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1:
                std::cout << "Enter message to send: "<< std::endl;
                std::getline(std::cin, message);
                message = base64_encode(message);
                send(socket, message.c_str(), message.size(), 0);
                std::cout << "Encrypted message has been sent" << std::endl;
                CreceiveAndProcessResponse(socket);
                break;
            case 2: {
                std::string filePath;
                std::cout << "Enter file path to send: ";
                std::cin.ignore(); // Add this line to clear the input buffer
                std::getline(std::cin, filePath);
                sendFile(socket, filePath);

                break;
            }
            case 3:
                send(socket, "exit", 4, 0);
                std::cout << "Exit command sent, closing connection" << std::endl;
                break;
            default:
                std::cout << "Invalid choice, please try again." << std::endl;
                break;
        }
    } while (choice != 3);
}


void startClient() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    SOCKET client_fd;
    sockaddr_in serv_addr;
    char buffer[1024] = { 0 };

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cout << "Socket creation error" << std::endl;
        return ;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    std::string ipAddress;
    std::cin.ignore();
    std::cout << "Enter the server's IP address: ";
    std::getline(std::cin, ipAddress);


    serv_addr.sin_addr.s_addr = inet_addr(ipAddress.c_str());

    if (connect(client_fd, reinterpret_cast<struct sockaddr*>(&serv_addr), sizeof(serv_addr)) < 0) {
        std::cout << "Connection Failed" << std::endl;
        return  ;
    }

    std::cout << "Connected to the server" << std::endl;

    clientMenu(client_fd);
    processResponse(client_fd,buffer);

    closesocket(client_fd);
    WSACleanup();

}
