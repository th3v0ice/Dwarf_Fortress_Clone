#pragma once
#include "Item.h"
#include <vector>
#include <memory>

class Armor : public Item
{
private:
	int defense_value;

public:
    static std::vector<std::string> armor_names;

	Armor(std::string _name, int value) : defense_value(value) {
		type = item_type::ITEM_TYPE_ARMOR;
		name = _name;
	};

	int get_defense() { return defense_value; }

	std::string toString(){
		std::string res = "[A] " + name + " (+" + std::to_string(defense_value) + " def)";
		return res;
	}

	static std::shared_ptr<Armor> generateArmor();
	
};