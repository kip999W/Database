#ifndef NAVIGATORMENU_H
#define NAVIGATORMENU_H

#include <windows.h>
#include <iostream>
#include <conio.h>
#include <vector>

class NavigatorMenu {
private:
    enum class Direction { ESC = 27, UP = 72, DOWN = 80, ENTER = 13 };

    HANDLE hStdOut;
    std::vector<std::string> items;
    int currentSelection;

    void goToXY(short x, short y);
    void consoleCursorVisible(bool show, short size);
    void display();

public:
    NavigatorMenu(const std::vector<std::string>& menuItems)
        : items(menuItems), currentSelection(0) { hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); }

    void close();
    void navigate();
    int getCurrentSelection() { return currentSelection; }

};

#endif // NAVIGATORMENU_H
