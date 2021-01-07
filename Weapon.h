#pragma once
#include "Item.h"
class Weapon :  public Item
{
private:
	int damage;

public:
	Weapon(std::string _name, int dmg) : damage(dmg) {
		type = item_type::ITEM_TYPE_WEAPON ;
		name = _name;
	}
	
	~Weapon() {}

	int get_damage() { return damage; }

	std::string toString(){
		std::string res = "[W] " + name + " (+" + std::to_string(damage) + " Att)";
		return res;
	}
};

