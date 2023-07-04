#include "FileTransfer.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <windows.h>



void fprintMessageWithDelay(const std::string& message, int delay) {
    std::cout << std::right << std::setw(30) << message << std::endl;
    for (char c : message) {
        std::cout << c;
        Sleep(delay); // Delay between characters
    }
    std::cout << std::endl;
}


void sendFile(SOCKET socket, const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filePath << std::endl;
        return;
    }

    char buffer[1024] = { 0 };
    std::string fileName = "file:" + filePath.substr(filePath.find_last_of("\\/") + 1);

    // Send the file name as the first part of the message
    send(socket, fileName.c_str(), fileName.size(), 0);

    while (file.read(buffer, sizeof(buffer))) {
        send(socket, buffer, file.gcount(), 0);
    }

    std::cout << "File sent: " << fileName << std::endl;
}


void receiveFile(SOCKET socket, const std::string& saveLocation) {
    std::ofstream file(saveLocation, std::ios::binary);
    if (!file) {
        std::cerr << "Unable to open received file" << std::endl;
        return;
    }

    char buffer[1024] = { 0 };

    while (true) {
        int valread = recv(socket, buffer, sizeof(buffer), 0);
        if (valread <= 0)
            break;
        file.write(buffer, valread);
    }

    std::string filesRecivedin = "File received and saved: " + saveLocation;
    fprintMessageWithDelay(filesRecivedin, 80);

}
