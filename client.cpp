#define _WIN32_WINNT 0x0600
#include <iostream>
#include <string>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <array>
#include <stdexcept>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define PORT 8080

uint8_t getOptimalCipher(int live_ping) {
    // Build the command: e.g., "python optimizer.py 150"
    std::string command = "python optimizer.py " + std::to_string(live_ping);
    std::array<char, 128> buffer;
    std::string result;

    // Open a pipe to the terminal and run the Python script
    FILE* pipe = _popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Error: Could not trigger Python brain. Defaulting to Tier 2." << std::endl;
        return 1; // Fallback to ChaCha20
    }

    // Capture the output (the single ID number)
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    _pclose(pipe);

    // Convert the string output to an actual 8-bit integer
    try {
        return static_cast<uint8_t>(std::stoi(result));
    } catch (...) {
        return 1; // Fallback on error
    }
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed. Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);    

    std::cout << "==========================================" << std::endl;
    std::cout << "  DYNAMIC CIPHER CLIENT - MANUAL OVERRIDE " << std::endl;
    std::cout << "==========================================" << std::endl;

    int user_ping;
    int packet_counter = 1;

    // Infinite loop so you can keep entering new pings
    while (true) {
        std::cout << "\nEnter live network ping in ms (or type -1 to quit): ";
        std::cin >> user_ping;

        // Exit condition
        if (user_ping < 0) {
            std::cout << "Shutting down client..." << std::endl;
            break;
        }
        
        std::cout << "[+] Asking Python Brain for optimal lock for " << user_ping << "ms..." << std::endl;
        
        // Call our new IPC function!
        uint8_t cipher_id = getOptimalCipher(user_ping);
        
        std::cout << "[+] Python Brain selected Cipher ID: " << (int)cipher_id << std::endl;

        // Pack the 2-bit Cipher ID into the top 2 bits of the first byte
        uint8_t header = (cipher_id & 0x03) << 6;

        // Create a unique payload string for every packet
        std::string payload_str = "Manual Datagram #" + std::to_string(packet_counter++);
        
        std::vector<char> packet;
        packet.push_back(static_cast<char>(header));
        packet.insert(packet.end(), payload_str.begin(), payload_str.end());

        // Fire the packet to the server
        int bytesSent = sendto(clientSocket, packet.data(), packet.size(), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "sendto failed. Error: " << WSAGetLastError() << std::endl;
        } else {
            std::cout << "[>>>] Sent '" << payload_str << "' with Cipher ID " << (int)cipher_id << " to the server." << std::endl;
        }
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}