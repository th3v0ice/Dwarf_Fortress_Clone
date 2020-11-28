#pragma once
#include "Item.h"
class Consumable : public Item
{
private:
	std::size_t amount;

public:
	Consumable(std::size_t a) : amount(a) {};
	~Consumable() {};

	int get_amount() { return amount; }
};

