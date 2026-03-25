#ifndef CIPHER_ENGINE_H
#define CIPHER_ENGINE_H

#include <vector>
#include <string>

class CipherEngine {
public:
    // Takes a raw buffer, parses the 2-bit ID from the highest bits of the first byte,
    // strips off the header byte, and routes the encrypted payload to the correct decryption tier.
    std::string processPacket(const std::vector<char>& buffer);
    std::string processPacket(const char* buffer, size_t length);

private:
    // Placeholder decryption methods
    std::string decryptTier1(const std::string& payload); // Represents AES
    std::string decryptTier2(const std::string& payload); // Represents ChaCha20
    std::string decryptTier3(const std::string& payload); // Represents Speck
};

#endif // CIPHER_ENGINE_H
