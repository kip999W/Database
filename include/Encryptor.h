#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <iostream>
#include <string>

class Encryptor {
private:
    std::string alphabet;
    int shift;

public:
    Encryptor(std::string alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz12345678906+-_!@#$%^&*(", int s = 3)
        : alphabet(alpha), shift(s) {}

    std::string encrypt(const std::string& text);
    std::string decrypt(const std::string& text);
    void setShift(int s) { shift = s; }

};

#endif // ENCRYPTOR_H
