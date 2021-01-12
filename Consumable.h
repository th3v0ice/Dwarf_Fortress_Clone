#pragma once
#include "Item.h"
#include <memory>

class Consumable : public Item
{
private:
	std::size_t amount;

public:
	Consumable(std::string _name, std::size_t a) : amount(a) {
		type = item_type::ITEM_TYPE_POTION;
		name = _name;
	};
	~Consumable() {};

	int get_amount() { return amount; }

	std::string toString(){
		std::string res = "[C] " + name + " (+" + std::to_string(amount) + " health)";
		return res;
	}

	static std::shared_ptr<Consumable> generateConsumable();
};

