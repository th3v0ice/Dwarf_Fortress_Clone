#pragma once

#include <vector>
#include "Player.h"
#include "Map.h"

#include "Consumable.h"
#include "Armor.h"
#include "Weapon.h"
#include "Monster.h"

#include <curses.h>

enum fields
{
    MONSTER,
    POTION,
    ARMOR,
    WEAPON,
    NONE
};

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
    DF_KEY_USE = 7,
    DF_KEY_STATS = 8
};

class View {
private:
    int 
        width,
        height, 
        x_cord,
        y_cord,
        prev_x,
        prev_y,
        center_x,
        center_y,
        limit_x,
        limit_y;

    bool 
        inv_open,
        stats_open;

    BUFFER buffer;

    std::shared_ptr<Player> shrdPlayer;
    std::shared_ptr<Map> shrdMap;

    int drawMap();
    int checkFieldAndPerfAction();
    int initiateFight(std::shared_ptr<Monster> monster);

    void printMessage(int x, int y, std::string s);

    void drawFightScreen(std::shared_ptr<Monster> monster, int& p_hp_x_coord, int& m_hp_x_coord, int& hp_y_coord);

    fields hash(data_t in) {
        if(in == L"M") return MONSTER;
        if(in == L"W") return WEAPON;
        if(in == L"A") return ARMOR;
        if(in == L"H") return POTION;

        return NONE;
    }

public:
    View(int w, int h, 
        std::shared_ptr<Map> m, 
        std::shared_ptr<Player> p): width(w), height(h), shrdMap(m), shrdPlayer(p) 
    {    
        x_cord = BORDER + m->getWidth() / 2;
        y_cord = BORDER + m->getHeight() / 2;
        prev_x = x_cord;
        prev_y = y_cord;
        center_x = width / 2;
        center_y = height / 2;
        limit_x = BORDER + m->getWidth() - 1;
        limit_y = BORDER + m->getHeight() - 1;

        inv_open = false;
        stats_open = false;
    }
    ~View(){}


    int gameLogic(gcode &code);
    
    int init();

};