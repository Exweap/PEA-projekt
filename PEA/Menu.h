//
// Created by lukasz on 16.10.2019.
//

#ifndef PEA_MENU_H
#define PEA_MENU_H

#include <iostream>

class Menu {
private:
    int optionsNum=23;
    std::string options[23];
public:
    Menu();
    void DisplayMenu();
    void EnterMenu();
    static void WaitForUser();
    static void ClearScreen();
    ~Menu();
};


#endif //PEA_MENU_H
