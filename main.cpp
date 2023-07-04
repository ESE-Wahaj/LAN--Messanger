#include <iostream>
#include "server.h"
#include "client.h"

int main() {
    int choice;

    std::cout << "1. Start server\n";
    std::cout << "2. Start client\n";
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    if (choice == 1) {
        startServer();
    } else if (choice == 2) {
        startClient();
    } else {
        std::cout << "Invalid choice!\n";
    }

    return 0;
}
