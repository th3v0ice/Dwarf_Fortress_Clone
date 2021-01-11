#pragma once

#include <vector>
#include "Player.h"
#include "Map.h"

#include "Consumable.h"
#include "Armor.h"
#include "Weapon.h"

typedef typename std::vector<std::vector<char>> BUFFER;


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

    std::vector<std::vector<char>> buffer;

    std::shared_ptr<Player> shrdPlayer;
    std::shared_ptr<Map> shrdMap;

public:
    View(int w, int h, 
        std::shared_ptr<Map> m, 
        std::shared_ptr<Player> p): width(w), height(h), shrdMap(m), shrdPlayer(p) 
    {    
        x_cord = width / 2;
        y_cord = height / 2;
        prev_x = x_cord;
        prev_y = y_cord;
        center_x = x_cord;
        center_y = y_cord;
        limit_x = width - 1;
        limit_y = height - 1;

        inv_open = false;
        stats_open = false;
    }
    ~View(){}


    int gameLogic(gcode &code);
    
    int init();
    int drawMap();

    int checkFieldAndPerfAction();
};