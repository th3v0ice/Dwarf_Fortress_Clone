// Dwarf_Fortress_Clone.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Map.h"
#include <memory>
#include <curses.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>


int inputHandler(int &x_cord, int &y_cord)
{
    int ch = 0;

    ch = getch();
    
    switch(ch)
    {
        case KEY_UP:
        case 119: //w 
        case 87:
            y_cord--;
            break;
        case KEY_DOWN: 
        case 115: //a
        case 65:
            //printw("\nDown Arrow");
            y_cord++;
            break;
        case KEY_LEFT: 
        case 97: //s
        case 83:
            //printw("\nLeft Arrow");
            x_cord--;
            break;
        case KEY_RIGHT: 
        case 100: //d
        case 68:
            //printw("\nRight Arrow");
            x_cord++;
            break;
        default:
        {
            if(ch != -1)
            {
                printw("\nThe pressed key is ");
                attron(A_BOLD);
                printw("%c --> %d", ch, ch);
                attroff(A_BOLD);
                break;
            }
            break;
        }
    }
    

    return 0;
}

int main()
{
    initscr();
    cbreak();
    noecho();
    clear();
    keypad(stdscr, true);
    nodelay(stdscr, TRUE);

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    printf ("lines %d\n", w.ws_row);
    printf ("columns %d\n", w.ws_col);

    std::vector<std::vector<char>> buffer;
    buffer.reserve(w.ws_row);
    buffer[0].reserve(w.ws_col);


    int 
        x_cord = 0,
        y_cord = 0,
        prev_x = x_cord,
        prev_y = y_cord;

    std::shared_ptr<Map> shrdMap = std::make_shared<Map>();

	int ret = shrdMap->loadMap("tests/good_test.json");
    if(ret < 0)
    {
        std::cout << "Failed while loading the Map file" << std::endl;
        return -1;
    }

    while(1)
    {
        inputHandler(x_cord, y_cord);

        if(prev_x != x_cord || prev_y != y_cord)
        {
            shrdMap->getMapAroundPlayer(x_cord, y_cord, w.ws_col, w.ws_row, buffer);
            prev_x = x_cord;
            prev_y = y_cord;
            shrdMap->draw_map(buffer);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    shrdMap->self_check();
}
