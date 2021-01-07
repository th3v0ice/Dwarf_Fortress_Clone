#pragma once
#include "Item.h"
class Armor : public Item
{
private:
	int defense_value;

public:
	Armor(std::string _name, int value) : defense_value(value) {
		type = item_type::ITEM_TYPE_ARMOR;
		name = _name;
	};

	int get_defense() { return defense_value; }

	std::string toString(){
		std::string res = "[A] " + name + " (+" + std::to_string(defense_value) + " def)";
		return res;
	}
};

