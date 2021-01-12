#include "Armor.h"

std::vector<std::string> Armor::armor_names = { 
        "Edge of Patience", 
        "Defense of Twilight",
        "Armor of Absorption",
        "Armor of Dominance",
        "Adamantite Cuirass",
        "Golden Armor",
        "Armor of Illusions",
        "Chestplate of Soul",
        "Mail Chestplate",
        "Vengeful Adamantite",
        "Tunic of Fury",
        "Protector of Souls",
        "Chestguard of Time"
    };


std::shared_ptr<Armor> Armor::generateArmor(){
    unsigned seed = time(0);
    srand(seed);
    int def = rand() % 200;
    int idx = rand() % armor_names.size();
    std::shared_ptr<Armor> armor(new Armor(armor_names[idx], def));
    return armor;
}