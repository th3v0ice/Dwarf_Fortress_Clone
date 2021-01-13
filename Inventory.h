#include <vector>
#include <memory>
#include "Item.h"
#include "Weapon.h"
#include "Armor.h"
#include "Consumable.h"
#include "Map.h"

#pragma once
class Inventory
{

public:
    Inventory()     : selected_item_idx(0), limit(3) { inventory.reserve(3);     }
    Inventory(int l): selected_item_idx(0), limit(l) { inventory.reserve(limit); }

    void drawInventory(BUFFER &buffer);
    void dropFromInventory();
    int addToInventory(std::shared_ptr<Item> item);

    void changeInventorySelection(int p, BUFFER &buffer);
    void setLimit(int lim) { limit = lim; }
    int getLimit() { return limit; }
    std::shared_ptr<Item> getSelectedItem();

    void fillWithDummyData(){
        std::shared_ptr<Weapon> w(new Weapon("Mighty sword", 100));
        std::shared_ptr<Armor> a(new Armor("Shiny armor", 20));
        std::shared_ptr<Consumable> c(new Consumable("Small potion", 10));

        std::shared_ptr<Item> w_i = w;
        std::shared_ptr<Item> a_i = a;
        std::shared_ptr<Item> c_i = c;

        inventory.push_back(w_i);
        inventory.push_back(a_i);
        inventory.push_back(c_i);
    }

    std::size_t size() { return inventory.size(); }
    void clear() { inventory.clear(); }
    item_type getItemTypeAtIndex(int i) { return inventory[i]->getType(); };
private:
    std::vector<std::shared_ptr<Item>> inventory;

    int selected_item_idx;
    int limit;
    //Coordinates for a star which designates selected item.
    //It will be much faster to change the selection instead 
    //of drawing everything again.
    int selected_item_idx_x;
    int selected_item_idx_y;
};

