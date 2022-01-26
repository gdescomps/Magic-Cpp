#pragma once

#include "../Ability.hpp"
#include "../Duel.hpp"
#include "../Player.hpp"

class Trample : public Ability {
public:
  std::string getName() const override {
    return "Trample";
  }
  
  std::string getDesc() const override {
    return "Deal excess damage to adversary.";
  }
    
  void usePostDuelAttack(Duel& duel) override {
    duel.adversary->addHP(-std::max(0, duel.attacker->getPower()));
  }
};

