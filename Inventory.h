#include <vector>
#include <memory>
#include "Item.h"
#include "Weapon.h"
#include "Armor.h"
#include "Consumable.h"

#pragma once
class Inventory
{

public:
    Inventory(){ inventory.reserve(3); }

    void drawInventory(std::vector<std::vector<char>> &buffer);
    void dropFromInventory();
    void addToInventory(Item *item);

    void changeInventorySelection(int p, std::vector<std::vector<char>> &buffer);

    std::shared_ptr<Item> getSelectedItem();

    void fillWithDummyData(){
        std::shared_ptr<Weapon> w(new Weapon("Mighty sword", 100));
        std::shared_ptr<Armor> a(new Armor("Shiny armor", 20));
        std::shared_ptr<Consumable> c(new Consumable("Great potion", 10));

        std::shared_ptr<Item> w_i = w;
        std::shared_ptr<Item> a_i = a;
        std::shared_ptr<Item> c_i = c;

        inventory.push_back(w_i);
        inventory.push_back(a_i);
        inventory.push_back(c_i);
    }
private:
    std::vector<std::shared_ptr<Item>> inventory;

    int selected_item_idx;

    //Coordinates for a star which designates selected item.
    //It will be much faster to change the selection instead 
    //of drawing everything again.
    int selected_item_idx_x;
    int selected_item_idx_y;
};

