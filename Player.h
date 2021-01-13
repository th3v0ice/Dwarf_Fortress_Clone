#pragma once
#include <sstream>
#include "Entity.h"
#include "Map.h"


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

	void drawInventory(BUFFER &buffer){
		inventory.drawInventory(buffer);
	}

    void changeInventorySelection(BUFFER &buffer, int p){
        inventory.changeInventorySelection(p, buffer);
        return;
    }

    void dropFromInventory(){
        inventory.dropFromInventory();
    }

    void useSelectedItemFromInventory();
    void drawCharacterStats(BUFFER &buffer);

    int addToInventory(std::shared_ptr<Item> item);

    static std::vector<std::string> shape;
};

