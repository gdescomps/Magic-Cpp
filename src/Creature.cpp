#include "Creature.hpp"


Creature::~Creature() {}

void Creature::attack(Duel* d){

    int attackerToughness = this->getToughness();
    int attackPower = this->getPower();

    for(auto blocker : d->getCardBlockers()){

        int blockerToughness = blocker->getToughness();
        attackerToughness = d->getCardAttacker()->getToughness();

        // The attacker attack if he has enough attacksPower
        if (attackPower > 0 && attackerToughness > 0) {
            if (attackPower < blockerToughness) {
                blockerToughness -= attackPower; // Reduce blocker toughness
                attackPower = 0; // Stop attacking
            }
            else {
                attackPower -= blockerToughness; // Keep attacking with reduced power
                blockerToughness = 0; // Kill blocker
            }
            blocker->setToughness(blockerToughness);
        }

        // The attacker fight each blocker even if he has no power anymore
        attackerToughness -= blocker->getPower();
        attackerToughness = attackerToughness < 0 ? 0 : attackerToughness;
    }

    this->setToughness(attackerToughness);
}
