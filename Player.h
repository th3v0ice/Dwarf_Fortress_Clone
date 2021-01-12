#pragma once
#include <sstream>
#include "Entity.h"



class Player : public Entity
{
public:
    Player(){
        name = "Player";
        health = 100;
        defense = 10;
        attack = 20;

        inventory.setLimit(5);
    }

	void drawInventory(std::vector<std::vector<char>> &buffer){
		inventory.drawInventory(buffer);
	}

    void changeInventorySelection(std::vector<std::vector<char>> &buffer, int p){
        inventory.changeInventorySelection(p, buffer);
        return;
    }

    void dropFromInventory(){
        inventory.dropFromInventory();
    }

    void useSelectedItemFromInventory();
    void drawCharacterStats(std::vector<std::vector<char>> &buffer);

    int addToInventory(std::shared_ptr<Item> item);

    static std::vector<std::string> shape;
};

