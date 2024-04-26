#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>

void handle_client(int clientSocket) {
    char buf[4096];

    while (true) {
        memset(buf, 0, 4096);
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == -1) {
            std::cerr << "Error in recv(). Quitting" << std::endl;
            break;
        }

        if (bytesReceived == 0) {
            std::cout << "Client disconnected " << std::endl;
            break;
        }

        send(clientSocket, buf, bytesReceived, 0);
    }

    close(clientSocket);
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Can't create a socket!" << std::endl;
        return -1;
    }

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(54000);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (sockaddr*)&server, sizeof(server));
    listen(server_fd, 5);

    while (true) {
        sockaddr_in client;
        socklen_t clientSize = sizeof(client);
        int clientSocket = accept(server_fd, (sockaddr*)&client, &clientSize);

        if (clientSocket == -1) {
            std::cerr << "Problem with client connecting!" << std::endl;
            continue;
        }

        std::thread clientThread(handle_client, clientSocket);
        clientThread.detach();
    }

    close(server_fd);

    return 0;
}
