#include "CipherEngine.h"
#include <stdexcept>

std::string CipherEngine::processPacket(const std::vector<char>& buffer) {
    if (buffer.empty()) {
        return "";
    }
    return processPacket(buffer.data(), buffer.size());
}

std::string CipherEngine::processPacket(const char* buffer, size_t length) {
    if (length == 0 || buffer == nullptr) {
        return "";
    }

    // The first byte of every received packet contains a 2-bit Cipher ID in its highest bits.
    unsigned char header = static_cast<unsigned char>(buffer[0]);
    unsigned char cipherId = (header >> 6) & 0x03; // Shift right by 6 to get the top 2 bits

    // Extract encrypted payload (strip off the header byte)
    std::string payload(buffer + 1, length - 1);

    // Route based on Cipher ID (00, 01, or 10)
    switch (cipherId) {
        case 0: // 00 -> Tier 1 (AES)
            return decryptTier1(payload);
        case 1: // 01 -> Tier 2 (ChaCha20)
            return decryptTier2(payload);
        case 2: // 10 -> Tier 3 (Speck)
            return decryptTier3(payload);
        default:
            throw std::runtime_error("Unknown Cipher ID in packet header.");
    }
}

std::string CipherEngine::decryptTier1(const std::string& payload) {
    // Placeholder implementation using XOR with a basic string key (e.g., 'A')
    std::string result = payload;
    for (char& c : result) {
        c ^= 'A';
    }
    return result;
}

std::string CipherEngine::decryptTier2(const std::string& payload) {
    // Placeholder implementation using XOR with a basic string key (e.g., 'B')
    std::string result = payload;
    for (char& c : result) {
        c ^= 'B';
    }
    return result;
}

std::string CipherEngine::decryptTier3(const std::string& payload) {
    // Placeholder implementation using XOR with a basic string key (e.g., 'C')
    std::string result = payload;
    for (char& c : result) {
        c ^= 'C';
    }
    return result;
}
