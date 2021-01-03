// Dwarf_Fortress_Clone.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Map.h"
#include <memory>
#include <stdio.h>  
#include <unistd.h>
#include <termios.h>

int getch(char* buf) {
        //char buf[2];// = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, buf, 3) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return 0;
}

int main()
{
    std::shared_ptr<Map> shrdMap = std::make_shared<Map>();
    std::cout << "Hello World!\n";
    char c[3];
    getch(c);

    shrdMap->self_check();
}
