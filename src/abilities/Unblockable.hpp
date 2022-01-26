#pragma once

#include <algorithm>

#include "../Ability.hpp"
#include "../Duel.hpp"
#include "../Creature.hpp"
#include "Reach.hpp"

class Unblockable : public Ability {
public:
  bool validateAttack(Duel const& duel) override {
    return duel.blockers.size() == 0;
  }
  
  std::string getName() const override {
    return "Unblockable";
  }
  
  std::string getDesc() const override {
    return "Cannot be blocked.";
  }
};
