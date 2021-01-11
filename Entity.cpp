#include "Entity.h"

Entity::Entity()
{
    name = "";
    health = 100;
    armor = nullptr;
    weapon = nullptr;
    attack = 10;
    defense = 0;
    inventory.setLimit(2);
}

int Entity::reduceHealth(int amount)
{
    health -= amount;
    if (health <= 0)
        return 1;

    return 0;
}

int Entity::equipOrConsume(std::shared_ptr<Item> i)
{
    switch (i->getType()) {
    case item_type::ITEM_TYPE_ARMOR:
    {
        std::shared_ptr<Armor> a_armor = std::dynamic_pointer_cast<Armor>(i);

        //We first unequip the previous armor if one existed
        if (armor)
            defense -= armor->get_defense();

        defense += a_armor->get_defense();

        armor = a_armor;
        break;
    }
    case item_type::ITEM_TYPE_POTION:
    {
        std::shared_ptr<Consumable> c_cons = std::dynamic_pointer_cast<Consumable>(i);
        health += c_cons->get_amount();
        if (health > 100)
            health = 100;
        break;
    }
    case item_type::ITEM_TYPE_WEAPON:
    {
        std::shared_ptr<Weapon> w_weapon = std::dynamic_pointer_cast<Weapon>(i);

        //Unequip a weapon first
        if (weapon)
            attack -= weapon->get_damage();

        attack += w_weapon->get_damage();

        weapon = w_weapon;
        break;
    }
    default:
    {
        break;
    }
    }

    return 0;
}
