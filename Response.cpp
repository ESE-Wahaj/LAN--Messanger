#include "FileTransfer.h"
#include "Response.h"
#include "Base64.h"
#include <iostream>
#include <iomanip>
#include <windows.h>
const std::string file_path = "G:\\## test\\Pf-ProjectFinal\\Received Files";
void processResponse(SOCKET socket, const std::string& buffer) {
    // Check if the response is a file or a message
    if (buffer.substr(0, 5) == "file:") {
        // The response is a file
        std::string saveLocation = file_path + "\\" + buffer.substr(5);
        receiveFile(socket, saveLocation);
    } else {
        // The response is a message

        std::string decryptedMessage = base64_decode(buffer);

        std::cout << std::endl;
        std::cout << std::right << std::setw(30) << "Encrypted Message Received:" << std::endl;
        std::cout << std::right << std::setw(30) << "\"";

        for (int i = 0; i < decryptedMessage.length(); i++) {
            std::cout << decryptedMessage[i];
            Sleep(80); // Delay between characters
        }
        std::cout<< "\"" << std::endl;
    }
}


