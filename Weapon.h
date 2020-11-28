#pragma once
#include "Item.h"
class Weapon :  public Item
{
private:
	int damage;

public:
	Weapon(int dmg) : damage(dmg) {}
	~Weapon() {}

	int get_damage() { return damage; }
};

