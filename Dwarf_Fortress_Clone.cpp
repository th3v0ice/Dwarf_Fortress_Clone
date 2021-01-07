// Dwarf_Fortress_Clone.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Map.h"
#include <memory>
#include <curses.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

#include "Player.h"



#define CLEAR printf("\033[H\033[J")
#define GOTOXY(x,y) printf("\033[%d;%dH", (y), (x))

int inputHandler(int &x_cord, int &y_cord, int limit_x, int limit_y, int& code)
{
    int ch = 0;

    ch = getch();
    
    switch(ch)
    {
        case KEY_UP:
        case 119: //w 
        case 87:
            y_cord--;
            if (y_cord < 0)
                y_cord = 0;
            break;
        case KEY_DOWN: 
        case 115: //a
        case 65:
            y_cord++;
            if(y_cord > limit_y)
                y_cord = limit_y;
            break;
        case KEY_LEFT: 
        case 97: //s
        case 83:
            x_cord--;
            if(x_cord < 0)
                x_cord = 0;
            break;
        case KEY_RIGHT: 
        case 100: //d
        case 68:
            x_cord++;
            if(x_cord > limit_x)
                x_cord = limit_x;
            break;
        case 109:
        case 77: //Menu
            code = 44;
            break;
        case 105:
        case 73: //Inventory
            code = 55;
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
    for(int i = 0; i < w.ws_row; i++)
        buffer[i].reserve(w.ws_col);


    int 
        x_cord = w.ws_col / 2,
        y_cord = w.ws_row / 2,
        prev_x = x_cord,
        prev_y = y_cord,
        center_x = x_cord,
        center_y = y_cord,
        code;

    Player player;
    player.testFillInventory();

    std::shared_ptr<Map> shrdMap = std::make_shared<Map>();

	int ret = shrdMap->loadMap("tests/mape.json");
    if(ret < 0)
    {
        std::cout << "Failed while loading the Map file" << std::endl;
        return -1;
    }




    while(1)
    {
        inputHandler(x_cord, y_cord, w.ws_col-1, w.ws_row-1, code);
        shrdMap->getMapAroundPlayer(x_cord, y_cord, w.ws_col, w.ws_row, buffer);

        if(code == 55)
        {
            player.drawInventory(buffer);
            shrdMap->draw_map(buffer);
            code = 0;
        }

        if(prev_x != x_cord || prev_y != y_cord)
        {
            shrdMap->getMapAroundPlayer(x_cord, y_cord, w.ws_col, w.ws_row, buffer);
            buffer[center_y][center_x] = 'P';
            prev_x = x_cord;
            prev_y = y_cord;
            CLEAR;
            GOTOXY(0, 0);
            shrdMap->draw_map(buffer);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    //shrdMap->self_check();
}
