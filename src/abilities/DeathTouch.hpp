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
  
  void usePreAttack(Duel& duel, Creature* blocker) override {
    if(duel.attacker->getPower() > 0) {
      attacked.push_back(blocker);
    }
  }
  
  void usePostDuelAttack(Duel&) override {
    for(Creature* c : attacked) {
      c->setToughness(0);
    }
    attacked = {};
  }
  
private:
  std::vector<Creature*> attacked;
};
