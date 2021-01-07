#pragma once
#include "Inventory.h"
#include "Weapon.h"
#include "Armor.h"
#include "Consumable.h"
#include "Item.h"
#include <string>
#include <memory>

class Entity
{
private:
	std::string name;
	Inventory inventory;

	Armor *armor;
	Weapon *weapon;

	int 
		health,
		attack,
		defense;

public:
	Entity();
	~Entity() {};

	/**
	* Reduces the health of the entity by the given amount.
	* If the health goes to zero or below zero, method will
	* return 1, which means that the entity is dead. Otherwise
	* it will return 0;
	*/
	int reduceHealth(int amount);

	std::string& getName() {
		return name;
	}

	int getHealth() {
		return health;
	}

	/**
	* Equip will calculate entities armor and attack values based on the
	* equiped item.
	*/
	int equipOrConsume(Item* i);

	void testFillInventory(){
		inventory.fillWithDummyData();
	}

	void drawInventory(std::vector<std::vector<char>> &buffer){
		inventory.drawInventory(buffer);
	}

};

