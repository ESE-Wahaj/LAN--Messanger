
#ifndef PF_PROJECTFINAL_RESPONSE_H
#define PF_PROJECTFINAL_RESPONSE_H
#include <winsock2.h>
#include <string>
void processResponse(SOCKET socket, const std::string& buffer);
void printMessageWithDelay(const std::string& message, int delay);
#endif //PF_PROJECTFINAL_RESPONSE_H
