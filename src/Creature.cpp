#include "Creature.hpp"


Creature::~Creature() {}

void Creature::attack(Creature* blocker) {
    int attackerToughness = this->getToughness();
    int attackerPower = this->getPower();
    int blockerToughness = blocker->getToughness();
    int blockerPower = blocker->getPower();
    
    // perform the attack simultaneously on the attacker & blocker
    this->setToughness(std::max(0, attackerToughness - blockerPower));
    blocker->setToughness(std::max(0, blockerToughness - attackerPower));
}
