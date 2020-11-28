#pragma once

#include <string>

enum class item_type {
	ITEM_TYPE_ARMOR = 0,
	ITEM_TYPE_WEAPON = 1,
	ITEM_TYPE_POTION = 2
};

class Item
{
private:
	std::string name;
	
	item_type type;


public:
	Item() {};
	virtual ~Item();

	item_type get_type() {
		return type;
	}

	void set_type(item_type itype) {
		type = itype;
	}
};

