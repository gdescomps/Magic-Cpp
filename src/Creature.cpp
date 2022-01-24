#include "Creature.hpp"
#include "Ability.hpp"
#include "abilities/DefaultAbility.hpp"

#include <algorithm>

Creature::Creature(int power, int toughness, std::vector<Ability*> abilities)
  : power(power), basePower(power), toughness(toughness), baseToughness(toughness), abilities(abilities)
{}

Creature::Creature(int power, int toughness)
  : Creature(power, toughness, { DefaultAbility::getInst() })
{}

void Creature::attack(Creature* blocker) {
  int attackerToughness = this->getToughness();
  int attackerPower = this->getPower();
  int blockerToughness = blocker->getToughness();
  int blockerPower = blocker->getPower();
  
  // perform the attack simultaneously on the attacker & blocker
  this->setToughness(std::max(0, attackerToughness - blockerPower));
  blocker->setToughness(std::max(0, blockerToughness - attackerPower));
}

DuelValidation Creature::validateAttack(Duel const& duel) {
  auto err = std::find_if(abilities.begin(), abilities.end(), [&] (Ability* ability) { return !ability->validateAttack(duel); });
  if(err == abilities.end()) return DuelValidation::ok(); 
  else return DuelValidation::ko(*err, this);
}

DuelValidation Creature::validateBlock(Duel const& duel) {
  auto err = std::find_if(abilities.begin(), abilities.end(), [&] (Ability* ability) { return !ability->validateBlock(duel, this); });
  if(err == abilities.end()) return DuelValidation::ok(); 
  else return DuelValidation::ko(*err, this);
}

bool Creature::hasAbility(Ability* ability) const {
  return std::ranges::find(abilities, ability) != abilities.end();
}

