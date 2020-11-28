#include "Entity.h"

Entity::Entity()
{
    name = "";
    health = 100;
    armor = nullptr;
    weapon = nullptr;
    attack = 10;
    defense = 0;
}

int Entity::reduce_health(int amount)
{
    health -= amount;
    if (health <= 0)
        return 1;

    return 0;
}

int Entity::equip_or_consume(Item& i)
{
    switch (i.get_type()) {
    case item_type::ITEM_TYPE_ARMOR:
    {
        Armor& a_armor = dynamic_cast<Armor&>(i);

        //We first unequip the previous armor if one existed
        if (armor)
            defense -= armor->get_defense();

        defense += a_armor.get_defense();

        armor = &a_armor;
        break;
    }
    case item_type::ITEM_TYPE_POTION:
    {
        Consumable& c_cons = dynamic_cast<Consumable&>(i);
        health += c_cons.get_amount();
        if (health > 100)
            health = 100;
        break;
    }
    case item_type::ITEM_TYPE_WEAPON:
    {
        Weapon& w_weapon = dynamic_cast<Weapon&>(i);

        //Unequip a weapon first
        if (weapon)
            attack -= weapon->get_damage();

        attack += w_weapon.get_damage();

        weapon = &w_weapon;
        break;
    }
    default:
    {
        break;
    }
    }

    return 0;
}
