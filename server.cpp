#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed. Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed. Error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "UDP Server listening on port " << PORT << "..." << std::endl;

    char buffer[BUFFER_SIZE];
    sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    while (true) {
        int bytesReceived = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&clientAddr, &clientAddrLen);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "recvfrom failed. Error: " << WSAGetLastError() << std::endl;
            continue;
        }

        if (bytesReceived > 0) {
            // Parse the 2-bit header (stored in the most significant bits of the first byte)
            uint8_t first_byte = static_cast<uint8_t>(buffer[0]);
            uint8_t cipher_id = (first_byte >> 6) & 0x03;
            
            // The payload starts at the next byte
            std::string payload;
            if (bytesReceived > 1) {
                payload.assign(&buffer[1], bytesReceived - 1);
            }

            std::cout << "Received datagram from client." << std::endl;
            std::cout << "Cipher ID (2-bit header): " << (int)cipher_id << std::endl;
            std::cout << "Payload: " << payload << std::endl;
            std::cout << "----------------------------------------" << std::endl;
        }
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
