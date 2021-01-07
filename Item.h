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
	
protected:
	item_type type;
	std::string name;


public:
	Item() {};
	virtual ~Item();

	item_type getType() {
		return type;
	}

	virtual std::string toString() = 0;

	void setType(item_type itype) {
		type = itype;
	}
};

