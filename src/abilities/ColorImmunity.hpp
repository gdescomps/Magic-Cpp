#pragma once

#include <algorithm>

#include "../Ability.hpp"
#include "../Duel.hpp"
#include "../Mana.hpp"
#include "../Creature.hpp"

template<Mana M>
class ColorImmunity : public Ability {
public:
  
  bool validateAttack(Duel const& duel) override {
    return std::all_of(duel.blockers.begin(), duel.blockers.end(), [] (Creature* c) {
      return c->getMana() != M;
    });
  }
  
  bool validateBlock(Duel const& duel, Creature* blocker) override {
    return blocker->getMana() != duel.attacker->getMana(); 
  }
  
  std::string getName() const override {
    return manaToString(M) + " Immunity";
  }
  
  std::string getDesc() const override {
    return "Cannot be attacked or inflict damage to " + manaToString(M) + " creatures.";
  }
  
  static ColorImmunity<M>* getInst() {
    static ColorImmunity<M> inst;
    return &inst;
  }
};
