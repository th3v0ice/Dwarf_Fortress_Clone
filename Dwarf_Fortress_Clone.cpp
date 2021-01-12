// Dwarf_Fortress_Clone.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Map.h"
#include <memory>
#include <curses.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

#include "View.h"
#include "Player.h"

gcode inputHandler()
{
    int ch = 0;

    ch = getch();
    
    switch(ch)
    {
        case KEY_UP:
        case 119: //w 
        case 87:  //W
            return gcode::DF_KEY_UP;
        case KEY_DOWN: 
        case 115: //a
        case 65:  //A
            return gcode::DF_KEY_DOWN;
        case KEY_LEFT: 
        case 97: //s
        case 83: //S
            return gcode::DF_KEY_LEFT;
        case KEY_RIGHT: 
        case 100: //d
        case 68:  //D
            return gcode::DF_KEY_RIGHT;
        case 109: //m
        case 77:  //M
            return gcode::DF_KEY_MENU;
        case 105: //i
        case 73:  //I
            return gcode::DF_KEY_INVENTORY;
        case 120: //x
        case 88:  //X
            return gcode::DF_KEY_DROP;
        case 117: //u
        case 85:  //U
            return gcode::DF_KEY_USE;
        case 99:  //c
        case 67:  //C
            return gcode::DF_KEY_STATS;
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
    

    return gcode::DF_KEY_NONE;
}


int main()
{
    struct winsize w;
    gcode code;
    std::shared_ptr<Map> shrdMap;
    std::shared_ptr<Player> player;
    std::shared_ptr<View> view;

    initscr();
    cbreak();
    noecho();
    clear();
    keypad(stdscr, true);
    nodelay(stdscr, TRUE);
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);     //Lets get terminal window size





    player = std::shared_ptr<Player>(new Player(), [](Player* plyr){ if(plyr) delete plyr;});
    player->testFillInventory();

    shrdMap = std::make_shared<Map>();

	int ret = shrdMap->loadMap("tests/mape.json");
    if(ret < 0)
    {
        std::cout << "Failed while loading the Map file" << std::endl;
        return -1;
    }

    view = std::shared_ptr<View>(new View(w.ws_col, w.ws_row, shrdMap, player));
    view->init();

    while(1)
    {
        code = inputHandler();
        if(view->gameLogic(code) < 0)
            break;
    }

    
}
