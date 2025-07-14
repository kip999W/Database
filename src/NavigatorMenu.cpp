#include "NavigatorMenu.h"

void NavigatorMenu::goToXY(short x, short y) {
    COORD coord = {x, y};
    SetConsoleCursorPosition(hStdOut, coord);
}

void NavigatorMenu::consoleCursorVisible(bool show, short size) {
    CONSOLE_CURSOR_INFO structCursorInfo;
    GetConsoleCursorInfo(hStdOut, &structCursorInfo);
    structCursorInfo.bVisible = show;
    structCursorInfo.dwSize = size;
    SetConsoleCursorInfo(hStdOut, &structCursorInfo);
}

void NavigatorMenu::display() {
    int x = 45, y = 13;
    for (int i = 0; i < items.size(); ++i) {
        if (i == currentSelection) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);

        goToXY(x, y++);
        std::cout << items[i] << std::endl;
    }
}

void NavigatorMenu::close() {
    system("cls");

    char ch;
    std::cout << "Вы действительно хотите выйти? (y/n): ";  std::cin >> ch;
    system("cls");

    if (ch == 'Y' || ch == 'y') exit(0);
}

void NavigatorMenu::navigate() {
    consoleCursorVisible(false, 100);

    char ch;
    while (true) {
        display();

        ch = _getch();
        if (ch == -32) ch = _getch();

        switch (ch) {
            case static_cast<char>(Direction::ESC):
                close();
                break;
            case static_cast<char>(Direction::UP):
                if (currentSelection > 0) --currentSelection;
                break;
            case static_cast<char>(Direction::DOWN):
                if (currentSelection < items.size() - 1) ++currentSelection;
                break;
            case static_cast<char>(Direction::ENTER):
                consoleCursorVisible(true, 100);
                system("cls");
                return;
                break;
        }
    }
}
