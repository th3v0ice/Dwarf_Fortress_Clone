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


    std::vector<std::string> armor_names;
    std::vector<std::string> weapon_names;

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

        armor_names.push_back("Edge of Patience");
        armor_names.push_back("Defense of Twilight");
        armor_names.push_back("Armor of Absorption");
        armor_names.push_back("Armor of Dominance");
        armor_names.push_back("Adamantite Cuirass");
        armor_names.push_back("Golden Armor");
        armor_names.push_back("Armor of Illusions");
        armor_names.push_back("Chestplate of Soul");
        armor_names.push_back("Mail Chestplate");
        armor_names.push_back("Vengeful Adamantite");
        armor_names.push_back("Tunic of Fury");
        armor_names.push_back("Protector of Souls");
        armor_names.push_back("Chestguard of Time");

        weapon_names.push_back("Tyrhung");
        weapon_names.push_back("Tranquility");
        weapon_names.push_back("King's Legacy");
        weapon_names.push_back("Firestorm Sword");
        weapon_names.push_back("Corrupted Blade");
        weapon_names.push_back("Cataclysm");
        weapon_names.push_back("Worldslayer");
        weapon_names.push_back("Espada");
        weapon_names.push_back("Windsong Protector");
        weapon_names.push_back("Spectral Sword");
        weapon_names.push_back("Armageddon");
        weapon_names.push_back("Severance");
    }
    ~View(){}


    int gameLogic(gcode &code);
    
    int init();
    int drawMap();

    int checkFieldAndPerfAction();
};