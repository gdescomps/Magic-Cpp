#include "Creature.hpp"
#include "Ability.hpp"

#include <algorithm>

Creature::Creature(int power, int toughness, std::vector<std::unique_ptr<Ability>> abilities)
  : power(power), basePower(power), toughness(toughness), baseToughness(toughness), abilities(move(abilities))
{}

Creature::Creature(int power, int toughness)
  : Creature(power, toughness, {})
{}

DuelValidation Creature::validateAttack(Duel const& duel) {
  auto err = std::find_if(abilities.begin(), abilities.end(), [&] (auto& ability) { return !ability->validateAttack(duel); });
  if(err == abilities.end()) return DuelValidation::ok(); 
  else return DuelValidation::ko(err->get(), this);
}

DuelValidation Creature::validateBlock(Duel const& duel) {
  auto err = std::find_if(abilities.begin(), abilities.end(), [&] (auto& ability) { return !ability->validateBlock(duel, this); });
  if(err == abilities.end()) return DuelValidation::ok(); 
  else return DuelValidation::ko(err->get(), this);
}

std::vector<Ability*> Creature::getAbilities() const {
  std::vector<Ability*> res(abilities.size());

  for(size_t i = 0; i < abilities.size(); i++) {
    res[i] = abilities[i].get();
  }

  return res;
}


void Creature::usePreAttack(Duel& duel, Creature* c) { for(auto& a : abilities) a->usePreAttack(duel, c); }
void Creature::usePostAttack(Duel& duel, Creature* c) { for(auto& a : abilities) a->usePostAttack(duel, c); }
void Creature::usePreBlock(Duel& duel) { for(auto& a : abilities)  a->usePreBlock(duel, this); }
void Creature::usePostBlock(Duel& duel) { for(auto& a : abilities) a->usePostBlock(duel, this); }
void Creature::usePreDuelAttack(Duel& duel) { for(auto& a : abilities) a->usePreDuelAttack(duel); }
void Creature::usePostDuelAttack(Duel& duel) { for(auto& a : abilities) a->usePostDuelAttack(duel); }
void Creature::usePreDuelBlock(Duel& duel) { for(auto& a : abilities) a->usePreDuelBlock(duel, this); }
void Creature::usePostDuelBlock(Duel& duel) { for(auto& a : abilities) a->usePostDuelBlock(duel, this); }

