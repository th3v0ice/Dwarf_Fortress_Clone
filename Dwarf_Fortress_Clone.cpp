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

enum class game_code
{
    KEY_NONE = -1,
    KEY_INVENTORY = 0,
    KEY_MENU = 1,
    KEY_DROP = 2
};

#define CLEAR printf("\033[H\033[J")
#define GOTOXY(x,y) printf("\033[%d;%dH", (y), (x))

int inputHandler(int &x_cord, int &y_cord, int limit_x, int limit_y, game_code& code)
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
            code = game_code::KEY_MENU;
            break;
        case 105:
        case 73: //Inventory
            code = game_code::KEY_INVENTORY;
            break;
        case 120: //Drop
            code = game_code::KEY_DROP;
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
        center_y = y_cord;
    
    game_code
        code;

    bool   
        inv_open = false;

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

        if(code == game_code::KEY_INVENTORY && !inv_open)
        {
            shrdMap->getMapAroundPlayer(x_cord, y_cord, w.ws_col, w.ws_row, buffer);

            player.drawInventory(buffer);
            shrdMap->draw_map(buffer);
            code = game_code::KEY_NONE;
            inv_open = true;
        }

        if(code == game_code::KEY_INVENTORY && inv_open)
        {
            //Closing the inventory
            inv_open = false;
            code = game_code::KEY_NONE;

            shrdMap->getMapAroundPlayer(x_cord, y_cord, w.ws_col, w.ws_row, buffer);
            buffer[center_y][center_x] = 'P';
            shrdMap->draw_map(buffer);

        }

        if(inv_open)
        {
            //Handle inventory selection change
            if(y_cord > prev_y)
                player.changeInventorySelection(buffer, 1);
            else if(y_cord < prev_y)
                player.changeInventorySelection(buffer, -1);
            
            if(code == game_code::KEY_DROP)
            {
                player.dropFromInventory();
                player.drawInventory(buffer);
                code = game_code::KEY_NONE;
            }

            shrdMap->draw_map(buffer);

            //Forbid moving while inventory is open
            x_cord = prev_x;
            y_cord = prev_y;
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
