#include "Consumable.h"


std::shared_ptr<Consumable> Consumable::generateConsumable() {
    unsigned seed = time(0);
    srand(seed);
    int hp = rand() % 100;
    std::string hpn = "Lesser Health Potion";
    if(hp > 80)
        hpn = "Greater Health Potion";
    else if(hp > 50)
        hpn = "Great Health Potion";

    std::shared_ptr<Consumable> cons(new Consumable(hpn, hp));

    return cons;
}