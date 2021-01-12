#pragma once
#include "Entity.h"
class Monster : public Entity
{

public:
    Monster(std::string _n, int att, int def) {
        name = _n;
        health = 100;
        attack = att;
        defense = def;
    };
    ~Monster(){};

    void fillInventoryWithRandomItems();

    static std::shared_ptr<Monster> generateMonster();

    void dropFromInventory() { inventory.clear(); };

    void itemsInInventory(int& w, int& a, int& c);
};

