#include "Weapon.h"

std::vector<std::string> Weapon::weapon_names = {
        "Tyrhung",
        "Tranquility",
        "King's Legacy",
        "Firestorm Sword",
        "Corrupted Blade",
        "Cataclysm",
        "Worldslayer",
        "Espada",
        "Windsong Protector",
        "Spectral Sword",
        "Armageddon",
        "Severance"
};

std::shared_ptr<Weapon> Weapon::generateWeapon() {
    unsigned seed = time(0);
    srand(seed);
    int att = rand() % 120;
    int idx = rand() % weapon_names.size();
    std::shared_ptr<Weapon> weapon(new Weapon(weapon_names[idx], att));
    return weapon;
}
