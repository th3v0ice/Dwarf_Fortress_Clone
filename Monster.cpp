#include "Monster.h"

/**
 * All ASCII Art was downloaded from
 *  
 * https://www.asciiart.eu/mythology
 * 
*/
creature_t Monster::centaur =  {
"  <=======]}======",
"    --.   /|",
"   _\"/_.'/",
" .'._._,.'",
" :/ \\{}/",
"(L  /--',----._",
"    |          \\\\",
"   : /-\\ .'-'\\ / |",
"    \\\\, ||    \\|",
"     \\/ ||    ||"
};

creature_t Monster::ghost = {
"       .-.",
"      ( \" )",
"   /\\_.' '._/\\",
"   |         |",
"    \\       /",
"     \\    /`",
"   (__)  /",
"   `.__.'"
};

creature_t Monster::gryphon = {
"    .-')",
"   (`_^ (    .----`/",
"    ` )  \\_/`   __/     __,",
"    __{   |`  __/      /_/",
"   / _{    \\__/ '--.  //",
"   \\_> \\_\\  >__/    \\((",
"        _/ /` _\\_   |))",
"       /__(  /______/`"
};

std::vector<creature_t> Monster::creatures = {centaur, ghost, gryphon};

void Monster::fillInventoryWithRandomItems() {
    unsigned seed = time(0);
    srand(seed);

    int inv_size = rand() % inventory.getLimit();

    for(int i = 0; i < inv_size; i++) {
        int itm_type = rand() % 3;

        switch(itm_type)
        {
            case 0: {
                std::shared_ptr<Armor> armor = Armor::generateArmor();
                inventory.addToInventory(std::static_pointer_cast<Item>(armor));
                break;
            }
            case 1: {
                std::shared_ptr<Weapon> weapon = Weapon::generateWeapon();
                inventory.addToInventory(std::static_pointer_cast<Item>(weapon));
                break;
            }
            case 2: {
                std::shared_ptr<Consumable> cons = Consumable::generateConsumable();
                inventory.addToInventory(std::static_pointer_cast<Item>(cons));
                break;
            }
        }
    }

    return;
}

std::shared_ptr<Monster> Monster::generateMonster() {
    unsigned seed = time(0);
    srand(seed);

    int att = rand() % 80;
    int def = rand() % 300;

    std::shared_ptr<Monster> monster(new Monster("Monster", att, def));
    monster->fillInventoryWithRandomItems();

    return monster;
}

void Monster::itemsInInventory(int& w, int& a, int& c) {
    w = 0;
    a = 0;
    c = 0;

    for(int i = 0; i < inventory.size(); i++) {
        item_type t = inventory.getItemTypeAtIndex(i);
        switch (t)
        {
        case item_type::ITEM_TYPE_ARMOR:
            a++;
            break;
        case item_type::ITEM_TYPE_WEAPON:
            w++;
            break;
        case item_type::ITEM_TYPE_POTION:
            c++;
            break;    
        default:
            break;
        }
    }

    return;
}