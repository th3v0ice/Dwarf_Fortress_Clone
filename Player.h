#pragma once
#include "Entity.h"
class Player : public Entity
{
public:
    Player(){
        name = "Player";
        health = 100;
        defense = 10;
        attack = 20;
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
};

