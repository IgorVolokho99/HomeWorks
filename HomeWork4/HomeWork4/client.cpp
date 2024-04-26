#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h> /

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        return 1;
    }

    int port = 54000;
    std::string ipAddress = "127.0.0.1";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1) {
        return 1;
    }

    char buf[4096];
    std::string userInput;
    do {
        std::cout << "> ";
        getline(std::cin, userInput);

        if (userInput.size() > 0) {
            int sendRes = send(sock, userInput.c_str(), userInput.size() + 1, 0);
            if (sendRes == -1) {
                std::cout << "Could not send to server!" << std::endl;
                continue;
            }

            memset(buf, 0, 4096);
            int bytesReceived = recv(sock, buf, 4096, 0);
            if (bytesReceived == -1) {
                std::cout << "There was an error getting response from server" << std::endl;
            } else {
                std::cout << "SERVER> " << std::string(buf, bytesReceived) << std::endl;
            }
        }
    } while(true);

    close(sock);

    return 0;
}
