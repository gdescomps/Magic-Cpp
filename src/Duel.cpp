#include "Duel.hpp"
#include "Creature.hpp"
#include "Player.hpp"

void Duel::performDuel() {
    for(Creature* blocker : blockers) {
      int attackerToughness = attacker->getToughness();
      int attackerPower = attacker->getPower();
      int blockerToughness = blocker->getToughness();
      int blockerPower = blocker->getPower();
      
      // perform the attack simultaneously on the attacker & blocker
      attacker->setToughness(std::max(0, attackerToughness - blockerPower));
      attacker->setPower(std::max(0, attackerPower - blockerToughness));
      blocker->setToughness(std::max(0, blockerToughness - attackerPower));
      blocker->setPower(std::max(0, blockerPower - attackerToughness));
    }
    
    // deal excess damage to the adversary
    // adversary->setHP(std::max(0, adversary->getHP() - attacker->getPower()));
  }
