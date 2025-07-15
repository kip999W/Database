#include "Encryptor.h"

std::string Encryptor::encrypt(const std::string& text) {
    std::string encryptedText;

    for (char c : text) {
        size_t pos = alphabet.find(c);
        if (pos != std::string::npos) {
            pos = (pos + shift) % alphabet.size();
            encryptedText += alphabet[pos];
        } else {
            encryptedText += c;
        }
    }

    return encryptedText;
}

std::string Encryptor::decrypt(const std::string& text) {
    std::string decryptedText;

    for (char c : text) {
        size_t pos = alphabet.find(c);
        if (pos != std::string::npos) {
            pos = (pos - shift + alphabet.size()) % alphabet.size();
            decryptedText += alphabet[pos];
        } else {
            decryptedText += c;
        }
    }

    return decryptedText;
}
