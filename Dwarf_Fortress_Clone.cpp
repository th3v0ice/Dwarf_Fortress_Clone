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

enum class gcode
{
    DF_KEY_NONE = -1,
    DF_KEY_INVENTORY = 0,
    DF_KEY_MENU = 1,
    DF_KEY_DROP = 2,
    DF_KEY_LEFT = 3,
    DF_KEY_RIGHT = 4,
    DF_KEY_UP = 5,
    DF_KEY_DOWN = 6,
    DF_KEY_USE = 7
};



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
    initscr();
    cbreak();
    noecho();
    clear();
    keypad(stdscr, true);
    nodelay(stdscr, TRUE);

    //Lets get terminal window size
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); 

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
        limit_x = w.ws_col - 1,
        limit_y = w.ws_row - 1;
    
    gcode
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

    shrdMap->getMapAroundPlayer(x_cord, y_cord, w.ws_col, w.ws_row, buffer);


    while(1)
    {
        code = inputHandler();

        switch(code)
        {
            case gcode::DF_KEY_INVENTORY: {
                if(!inv_open) {
                    inv_open = true;
                    shrdMap->getMapAroundPlayer(x_cord, y_cord, w.ws_col, w.ws_row, buffer);
                    player.drawInventory(buffer);
                } else {
                    //Closing the inventory
                    inv_open = false;
                    shrdMap->getMapAroundPlayer(x_cord, y_cord, w.ws_col, w.ws_row, buffer);
                    buffer[center_y][center_x] = 'P';
                }
                break;
            }
            case gcode::DF_KEY_DROP:
            {
                if(inv_open){
                    player.dropFromInventory();
                    player.drawInventory(buffer);
                }
                break;
            }
            case gcode::DF_KEY_USE: {
                if(inv_open) {
                    player.useSelectedItemFromInventory();
                    player.drawInventory(buffer);
                }
                break;
            }
            case gcode::DF_KEY_UP: {
                if(inv_open){
                    player.changeInventorySelection(buffer, -1);
                    y_cord = prev_y;        //Forbid moving while inventory is open
                } else {
                    y_cord--;
                    if (y_cord < 0)
                        y_cord = 0;

                    shrdMap->getMapAroundPlayer(x_cord, y_cord, w.ws_col, w.ws_row, buffer);
                    buffer[center_y][center_x] = 'P';
                    prev_y = y_cord;
                }
                break;
            }
            case gcode::DF_KEY_DOWN: {
                if(inv_open){
                    player.changeInventorySelection(buffer, 1);
                    y_cord = prev_y;        //Forbid moving while inventory is open
                } else {
                    y_cord++;
                    if(y_cord > limit_y)
                        y_cord = limit_y;

                    shrdMap->getMapAroundPlayer(x_cord, y_cord, w.ws_col, w.ws_row, buffer);
                    buffer[center_y][center_x] = 'P';
                    prev_y = y_cord;
                }
                break;                
            }
            case gcode::DF_KEY_LEFT: {
                if(inv_open){
                    x_cord = prev_x;
                } else {
                    x_cord--;
                    if(x_cord < 0)
                        x_cord = 0;
                
                    shrdMap->getMapAroundPlayer(x_cord, y_cord, w.ws_col, w.ws_row, buffer);
                    buffer[center_y][center_x] = 'P';
                    prev_x = x_cord;
                }
                break;
            }
            case gcode::DF_KEY_RIGHT: {
                if(inv_open){
                    x_cord = prev_x;
                } else {
                    x_cord++;
                    if(x_cord > limit_x)
                        x_cord = limit_x;

                    shrdMap->getMapAroundPlayer(x_cord, y_cord, w.ws_col, w.ws_row, buffer);
                    buffer[center_y][center_x] = 'P';
                    prev_x = x_cord;
                }
                break;                
            }
        }

        shrdMap->draw_map(buffer); //Its not neccessary to redraw everytime
        code = gcode::DF_KEY_NONE;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    //shrdMap->self_check();
}
