#pragma once
#include "Item.h"
class Armor : public Item
{
private:
	int defense_value;

public:
	Armor(int value) : defense_value(value) {};

	int get_defense() { return defense_value; }
};

