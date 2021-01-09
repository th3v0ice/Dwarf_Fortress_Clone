#pragma once
#include "Entity.h"
class Player : public Entity
{
public:
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
};

