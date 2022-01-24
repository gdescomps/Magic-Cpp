#pragma once

#include "../Ability.hpp"
#include "../Duel.hpp"
#include "../Creature.hpp"

class DefaultAbility : public Ability {
public:
  bool validateAttack(Duel const& duel) override {
    return !duel.attacker->isFirstTurn();
  }

  bool validateBlock(Duel const&, Creature*) override {
    return true;
  }
  
  std::string getName() const override { return "Default"; }
  
  static DefaultAbility* getInst() {
    static DefaultAbility inst;
    return &inst;
  }
  
private:
};
