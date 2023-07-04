
#ifndef PF_PROJECTFINAL_FILETRANSFER_H
#define PF_PROJECTFINAL_FILETRANSFER_H
#include <winsock2.h>
#include <string>

void sendFile(SOCKET socket, const std::string& filePath);
void receiveFile(SOCKET socket, const std::string& saveLocation);

#endif //PF_PROJECTFINAL_FILETRANSFER_H


