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
    return "Deal excess damage to adversary";
  }
  
  
  void usePostDuel(Duel& duel, Creature* creature) override {
    if(creature == duel.attacker) {
      auto hp = duel.adversary->getHP();
      duel.adversary->setHP(std::max(0, hp - creature->getPower()));
    }
  }
  
  static Trample* getInst() {
    static Trample inst;
    return &inst;
  }
};

