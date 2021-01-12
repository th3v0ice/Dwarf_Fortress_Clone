#pragma once
#include "Entity.h"
#include <vector>
#include <memory>
#include <map>

typedef std::vector<std::string> creature_t;

class Monster : public Entity
{
private:
    static creature_t centaur;
    static creature_t ghost;
    static creature_t gryphon;


    static std::vector<creature_t> creatures;

    creature_t shape;

public:
    Monster(std::string _n, int att, int def) {
        name = _n;
        health = 100;
        attack = att;
        defense = def;

        unsigned seed = time(0);
        srand(seed);
        shape = creatures[rand() % 3];
    };
    ~Monster(){};

    void fillInventoryWithRandomItems();

    static std::shared_ptr<Monster> generateMonster();

    void dropFromInventory() { inventory.clear(); };

    void itemsInInventory(int& w, int& a, int& c);

    creature_t getShape() { return shape; }
};

