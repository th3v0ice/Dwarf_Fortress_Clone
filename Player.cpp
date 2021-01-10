#include "Player.h"


void Player::useSelectedItemFromInventory() {
    std::shared_ptr<Item> item = inventory.getSelectedItem();
    
    if(!item) 
        return;


    inventory.dropFromInventory();
    equipOrConsume(item);
    
    return;
}