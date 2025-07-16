#include <iostream>
#include <fstream>
#include <regex>

#include "NavigatorMenu.h"
#include "Encryptor.h"

bool checkFileExists(const std::string& filename);

class Database {
private:
    struct UserInfo {
        std::string username;
        std::string surname;
        std::string patronymic;
        std::string group;
        std::string phoneNumber;
        int day, month, year;
    };

    const std::string f_DATABASE = "./files/database/";
    UserInfo* users;
    int numberOfData = 0;

    bool isValidPhoneNumber(const std::string& phoneNumber);
    bool isValidDate(int day, int month, int year);

    void addUser(UserInfo* (&users), int& n);
    void dataReading(UserInfo* (&users), int& n, const std::string& filename);
    void dataChange(UserInfo* (&users), int& n);
    void saveData(UserInfo* users, int n, const std::string& filename);
    void copyData(UserInfo* (&data_new), UserInfo* (&data_old), int n);
    void deleteData(UserInfo* (&data), int& n);
    void printUser(UserInfo* (&users), int& n) const;

public:
    Database() {
        users = new UserInfo[numberOfData];
    }

    void displayDatabaseMenu();

    ~Database() {
        delete[] users;
    }

};

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

bool Database::isValidPhoneNumber(const std::string& phoneNumber) {
    std::regex phoneRegex(R"(^(\+?\d{1,3}[- ]?)?\d{10}$)");
    return std::regex_match(phoneNumber, phoneRegex);
}

bool Database::isValidDate(int day, int month, int year) {
    if (year < 1900 || year > std::time(nullptr)) {
        std::cout << "��� ��� ����������� ���������!" << std::endl;
        system("pause");
        system("cls");
        return false;
    }

    if (month < 1 || month > 12) {
        std::cout << "�������� �����!" << std::endl;
        system("pause");
        system("cls");
        return false;
    }

    int daysInMonth[] = {31, (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    return day > 0 && day <= daysInMonth[month - 1];
}

void Database::addUser(UserInfo* (&users), int& n) {
    do {
        std::cout << "������� ���-�� ������ (�� 1 �� 30): "; std::cin >> n;
        system("cls");

        if (n < 1 || n > 30) {
            std::cout << "���������� ��� ���!" << std::endl;
            system("pause");
            system("cls");
        }

    } while (n < 1 || n > 30);

    users = new UserInfo[n];

    for (int i = 0; i < n; i++) {
        bool isValid;

        do {
            isValid = true;

            std::cout << "������� ���: "; std::cin >> users[i].username;
            std::cout << "������� �������: "; std::cin >> users[i].surname;
            std::cout << "������� ��������: "; std::cin >> users[i].patronymic;
            std::cout << "������� ������: "; std::cin >> users[i].group;
            std::cout << "������� ����� ��������: "; std::cin >> users[i].phoneNumber;

            if (!isValidPhoneNumber(users[i].phoneNumber)) {
                std::cout << "�������� ����� ��������. ���������� ��� ���." << std::endl << std::endl;
                isValid = false;
                continue;
            }

            std::cout << "������� ���� �������� (��): "; std::cin >> users[i].day;
            std::cout << "������� ����� �������� (��): "; std::cin >> users[i].month;
            std::cout << "������� ��� �������� (����): "; std::cin >> users[i].year;

            if (!isValidDate(users[i].day, users[i].month, users[i].year))
                isValid = false;

        } while (!isValid);

        system("cls");
    }

    std::cout << "������ ������� ���������!" << std::endl;
}

void Database::dataReading(UserInfo* (&users), int& n, const std::string& filename) {
    Encryptor encryptor("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz12345678906+-_!@#$%^&*(", 7);
    std::ifstream inFile(filename);

    if (inFile.is_open()) {
        std::string encryptedN;
        inFile >> encryptedN;
        n = std::stoi(encryptor.decrypt(encryptedN));

        users = new UserInfo[n];

        for (int i = 0; i < n; i++) {
            std::string encryptedSurname, encryptedUsername, encryptedPatronymic;
            std::string encryptedGroup;
            std::string encryptedPhoneNumber;
            std::string encryptedDay, encryptedMonth, encryptedYear;

            inFile >> encryptedSurname;
            inFile >> encryptedUsername;
            inFile >> encryptedPatronymic;
            inFile >> encryptedGroup;
            inFile >> encryptedPhoneNumber;
            inFile >> encryptedDay;
            inFile >> encryptedMonth;
            inFile >> encryptedYear;

            users[i].surname = encryptor.decrypt(encryptedSurname);
            users[i].username = encryptor.decrypt(encryptedUsername);
            users[i].patronymic = encryptor.decrypt(encryptedPatronymic);
            users[i].group = encryptor.decrypt(encryptedGroup);
            users[i].phoneNumber = encryptor.decrypt(encryptedPhoneNumber);
            users[i].day = std::stoi(encryptor.decrypt(encryptedDay));
            users[i].month = std::stoi(encryptor.decrypt(encryptedMonth));
            users[i].year = std::stoi(encryptor.decrypt(encryptedYear));
        }

        std::cout << "������ ������� ���������!" << std::endl;
    } else
        std::cerr << "������ ��� ������� �������� �����!" << std::endl;

    inFile.close();
}

void Database::dataChange(UserInfo* (&users), int& n) {
    int _n;
    std::cout << "������� ����� ��������, ������� �� ������ �������� (�� 1 �� " << n << "): ";
    std::cin >> _n;
    _n--;

    system("cls");

    if (_n >= 0 && _n < n) {
        bool isValid;

        do {
            isValid = true;

            std::cout << "������� ���: "; std::cin >> users[_n].username;
            std::cout << "������� �������: "; std::cin >> users[_n].surname;
            std::cout << "������� ��������: "; std::cin >> users[_n].patronymic;
            std::cout << "������� ������: "; std::cin >> users[_n].group;
            std::cout << "������� ����� ��������: "; std::cin >> users[_n].phoneNumber;

            if (!isValidPhoneNumber(users[_n].phoneNumber)) {
                std::cout << "�������� ����� ��������. ���������� ��� ���." << std::endl << std::endl;
                isValid = false;
                continue;
            }

            std::cout << "������� ���� �������� (��): "; std::cin >> users[_n].day;
            std::cout << "������� ����� �������� (��): "; std::cin >> users[_n].month;
            std::cout << "������� ��� �������� (����): "; std::cin >> users[_n].year;

            if (!isValidDate(users[_n].day, users[_n].month, users[_n].year))
                isValid = false;

        } while (!isValid);

        system("cls");

        std::cout << "������ ��������!" << std::endl;
    } else
        std::cout << "��������� ����� ��������!" << std::endl;
}

void Database::copyData(UserInfo* (&data_new), UserInfo* (&data_old), int n) {
    for (int i = 0; i < n; i++)
        data_new[i] = data_old[i];
}

void Database::deleteData(UserInfo* (&data), int& n) {
    int _n;
    std::cout << "������� ����� ��������, ������� �� ������ ������� (�� 1 �� " << n << "): ";
    std::cin >> _n;
    _n--;

    system("cls");

    if (_n >= 0 && _n < n) {
        UserInfo* buf = new UserInfo[n];

        copyData(buf, data, n);

        --n;
        data = new UserInfo[n];

        int q = 0;
        for (int i = 0; i <= n; ++i) {
            if (i != _n) {
                data[q] = buf[i];
                ++q;
            }
        }

        delete[] buf;

        std::cout << "������ �������!" << std::endl;
    } else
        std::cout << "������!" << std::endl;
}

void Database::saveData(UserInfo* users, int n, const std::string& filename) {
    Encryptor encryptor("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz12345678906+-_!@#$%^&*(", 7);
    std::ofstream outFile(filename);

    if (outFile.is_open()) {
        outFile << encryptor.encrypt(std::to_string(n)) << std::endl;

        for (int i = 0; i < n; i++) {
            outFile << encryptor.encrypt(users[i].surname) << std::endl
                    << encryptor.encrypt(users[i].username) << std::endl
                    << encryptor.encrypt(users[i].patronymic) << std::endl
                    << encryptor.encrypt(users[i].group) << std::endl
                    << encryptor.encrypt(users[i].phoneNumber) << std::endl
                    << encryptor.encrypt(std::to_string(users[i].day)) << std::endl
                    << encryptor.encrypt(std::to_string(users[i].month)) << std::endl
                    << encryptor.encrypt(std::to_string(users[i].year)) << std::endl;
        }

        std::cout << "������ ���������!" << std::endl;
    } else
        std::cerr << "������ ��� ������� �������� �����!" << std::endl;

    outFile.close();
}

void Database::printUser(UserInfo* (&users), int& n) const {
    std::cout << "���-�� ������: " << n << std::endl << std::endl;

    for (int i = 0; i < n; i++) {
        std::cout << "���������� #" << i + 1 << std::endl;
        std::cout << "�������: " << users[i].surname << std::endl;
        std::cout << "���: " << users[i].username << std::endl;
        std::cout << "��������: " << users[i].patronymic << std::endl;
        std::cout << "������: " << users[i].group << std::endl;
        std::cout << "����� ��������: " << users[i].phoneNumber << std::endl;
        std::cout << "��.��.�� ��������: " << users[i].day << "." << users[i].month << "." << users[i].year << std::endl;
        std::cout << "_______________________________________________" << std::endl;
    }

    system("pause");
    system("cls");
}

void Database::displayDatabaseMenu() {
    SetConsoleTitle("Database");

    std::vector<std::string> items = {
        "�������� ������",
        "������ ������",
        "�������� ������",
        "������� ������",
        "��������� ������",
        "������� ������",
        "�����"
    };

    NavigatorMenu navigatorMenu(items);
    navigatorMenu.navigate();
    int currentSelection = navigatorMenu.getCurrentSelection();

    std::string filename;

    switch (currentSelection) {
        case 0:
            addUser(users, numberOfData);
            break;
        case 1:
            std::cout << "������� �������� �����: ";
            std::cin >> filename;

            system("cls");

            dataReading(users, numberOfData, f_DATABASE + filename);
            break;
        case 2:
            if (numberOfData != 0) {
                dataChange(users, numberOfData);
            } else
                std::cout << "������ �����!" << std::endl;
            break;
        case 3:
            if (numberOfData != 0) {
                deleteData(users, numberOfData);
            } else
                std::cout << "������ �����!" << std::endl;
            break;
        case 4:
            if (numberOfData != 0) {
                std::cout << "���������� �������� ��� �����: ";
                std::cin >> filename;

                system("cls");

                saveData(users, numberOfData, f_DATABASE + filename);
            } else
                std::cout << "������ �����!" << std::endl;
            break;
        case 5:
            printUser(users, numberOfData);
            break;
        case 6:
            navigatorMenu.close();
            break;
    }
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
                Database database;
                while (true)
                    database.displayDatabaseMenu();
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
