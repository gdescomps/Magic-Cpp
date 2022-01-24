#pragma once

#include <algorithm>

#include "../Ability.hpp"
#include "../Duel.hpp"
#include "../Creature.hpp"
#include "Reach.hpp"

class Flying : public Ability {
public:
  bool validateAttack(Duel const& duel) override {
    Ability* reach = Reach::getInst();
    return std::all_of(duel.blockers.begin(), duel.blockers.end(), [=] (Creature* b) {
      return b->hasAbility(this) || b->hasAbility(reach);
    });
  }
  
  std::string getName() const override {
    return "Flying";
  }
  
  std::string getDesc() const override {
    return "Can only be blocked by Flying or Reach creatures.";
  }
  
  static Flying* getInst() {
    static Flying inst;
    return &inst;
  }
};


