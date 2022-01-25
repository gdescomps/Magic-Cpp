#pragma once

#include "../Ability.hpp"
#include "../Duel.hpp"
#include "../Creature.hpp"

class DeathTouch : public Ability {
public:
  std::string getName() const override {
    return "DeathTouch";
  }
  
  std::string getDesc() const override {
    return "Can block Flying creatures.";
  }
  
  void usePostAttack(Duel&, Creature* blocker) override {
    blocker->setToughness(0);
  }
  
  static DeathTouch* getInst() {
    static DeathTouch inst;
    return &inst;
  }
};



