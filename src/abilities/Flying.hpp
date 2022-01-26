#pragma once

#include <algorithm>

#include "../Ability.hpp"
#include "../Duel.hpp"
#include "../Creature.hpp"
#include "Reach.hpp"

class Flying : public Ability {
public:
  bool validateAttack(Duel const& duel) override {
    return std::all_of(duel.blockers.begin(), duel.blockers.end(), [&] (Creature* b) {
      return b->hasAbility<Flying>() || b->hasAbility<Reach>();
    });
  }
  
  std::string getName() const override {
    return "Flying";
  }
  
  std::string getDesc() const override {
    return "Can only be blocked by Flying or Reach creatures.";
  }
};
