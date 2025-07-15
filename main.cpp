#include <iostream>
#include <fstream>

#include "NavigatorMenu.h"
#include "Encryptor.h"

bool checkFileExists(const std::string& filename);

class Authorization {
private:
    static const std::string f_AUTHORIZATION_INPUT;
    static const std::string f_AUTHORIZATION_KEYWORD;

    std::string readEncryptedFile(const std::string& filename, int lineNumber);
    void writeEncryptedFile(const std::string& filename, const std::string& data);

    bool entrance();
    bool validateInput(std::string& input);
    void registerUser();
    bool restoreUser();

public:
    void displayAuthorization();

};

const std::string Authorization::f_AUTHORIZATION_INPUT = "./files/authorization/input.txt";
const std::string Authorization::f_AUTHORIZATION_KEYWORD = "./files/authorization/keyword.txt";

int main() {
    setlocale(LC_ALL, "");

    Authorization aut;
    while (true)
        aut.displayAuthorization();

    return 0;
}

bool checkFileExists(const std::string& filename) {
    std::ifstream inFile(filename);
    return inFile.is_open();
}

std::string Authorization::readEncryptedFile(const std::string& filename, int lineNumber) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "������ ��� ������� ������� ����: " << filename << std::endl;
        return "";
    }

    std::string line;
    for (int i = 0; i <= lineNumber; i++)
        std::getline(infile, line);

    infile.close();
    return line;
}

void Authorization::writeEncryptedFile(const std::string& filename, const std::string& data) {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "������ ��� ������� ������� ���� ��� ������ (" << filename << ")" << std::endl;
        return;
    }

    outfile << data;
    outfile.close();
}

bool Authorization::entrance() {
    std::string username, password;

    std::cout << "+===== ����������� =====+" << std::endl;
    std::cout << "������� �����: ";
    std::cin >> username;
    std::cout << "������� ������: ";
    std::cin >> password;

    system("cls");

    Encryptor encryptor("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz12345678906+-_!@#$%^&*(", 7);
    std::string decryptUsername = encryptor.decrypt(readEncryptedFile(f_AUTHORIZATION_INPUT, 0));
    std::string decryptPassword = encryptor.decrypt(readEncryptedFile(f_AUTHORIZATION_INPUT, 1));

    if (username == decryptUsername) {
        std::cout << "����� ����������!" << std::endl;
        return true;
    } else {
        std::cout << "�������� ����� ��� ������!" << std::endl;
        return false;
    }
}

bool Authorization::validateInput(std::string& input) {
    const int MIN_LENGTH = 5;
    const int MAX_LENGTH = 25;

    if (input.size() < MIN_LENGTH || input.size() > MAX_LENGTH) {
        std::cout << "���������� ������ ����!" << std::endl;
        system("pause");
        system("cls");
        return false;
    }

    bool hasUpper = false, hasLower = false, hasDigit = false, hasOther = false;
    for (char& el : input) {
        if (std::isupper(el)) hasUpper = true;
        else if (std::islower(el)) hasLower = true;
        else if (std::isdigit(el)) hasDigit = true;
        else hasOther = true;
    }

    return (hasUpper + hasLower + hasDigit + hasOther) >= 3;
}

void Authorization::registerUser() {
    if (checkFileExists(f_AUTHORIZATION_INPUT)) {
        std::cerr << "� ��� ��� ���� �������!" << std::endl;
        return;
    }

    std::string username, password, keyword;

    std::cout << "+===== ����������� =====+" << std::endl;
    std::cout << "���������� �����: ";
    std::cin >> username;
    std::cout << "���������� ������: ";
    std::cin >> password;
    std::cout << "�������� ������� �����: ";
    std::cin >> keyword;

    system("cls");

    if (validateInput(username) && validateInput(password) && validateInput(keyword)) {
        Encryptor encryptor("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz12345678906+-_!@#$%^&*(", 7);

        writeEncryptedFile(f_AUTHORIZATION_INPUT, encryptor.encrypt(username) + "\n" + encryptor.encrypt(password));
        writeEncryptedFile(f_AUTHORIZATION_KEYWORD, encryptor.encrypt(keyword));

        std::cout << "����������� ������ �������!" << std::endl;
    }
}

bool Authorization::restoreUser() {
    std::string keyword;

    std::cout << "+===== �������������� =====+" << std::endl;
    std::cout << "������� ������� �����: ";
    std::cin >> keyword;

    system("cls");

    Encryptor encryptor("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz12345678906+-_!@#$%^&*(", 7);
    std::string oldKeyword = encryptor.decrypt(readEncryptedFile(f_AUTHORIZATION_KEYWORD, 0));

    if (keyword != oldKeyword) {
        std::cout << "�������� ������� �����!" << std::endl;
        return false;
    }

    std::string oldUsername = encryptor.decrypt(readEncryptedFile(f_AUTHORIZATION_INPUT, 0));
    std::string oldPassword = encryptor.decrypt(readEncryptedFile(f_AUTHORIZATION_INPUT, 1));

    std::string newUsername, newPassword, newKeyword;
    do {
        std::cout << "���������� ����� �����: ";
        std::cin >> newUsername;
        std::cout << "���������� ����� ������: ";
        std::cin >> newPassword;
        std::cout << "���������� ����� ������� �����: ";
        std::cin >> newKeyword;

        system("cls");

        if (newUsername == oldUsername) std::cout << "���������� ������ �����!" << std::endl << std::endl;
        else if (newPassword == oldPassword) std::cout << "���������� ������ ������!" << std::endl << std::endl;
        else if (newKeyword == oldKeyword) std::cout << "���������� ������ ������� �����!" << std::endl << std::endl;
    } while (newUsername == oldUsername || newPassword == oldPassword || newKeyword == oldKeyword);

    if (validateInput(newUsername) && validateInput(newPassword) && validateInput(newKeyword)) {
        writeEncryptedFile(f_AUTHORIZATION_INPUT, encryptor.encrypt(newUsername) + "\n" + encryptor.encrypt(newPassword));
        writeEncryptedFile(f_AUTHORIZATION_KEYWORD, encryptor.encrypt(newKeyword));

        std::cout << "������ ������� �������������!" << std::endl;
        return true;
    }
}

void Authorization::displayAuthorization() {
    SetConsoleTitle("Authorization");

    std::vector<std::string> items = { "�����������", "�����������", "��������������", "�����" };
    NavigatorMenu navigatorMenu(items);
    navigatorMenu.navigate();
    int currentSelection = navigatorMenu.getCurrentSelection();

    switch (currentSelection) {
        case 0:
            if (entrance()) {

            }

            break;
        case 1:
            registerUser();
            break;
        case 2:
            restoreUser();
            break;
        case 3:
            navigatorMenu.close();
            break;
    }
}
