#pragma once

#include "../Ability.hpp"
#include "../Duel.hpp"
#include "../Creature.hpp"
#include "../Player.hpp"
#include "Trample.hpp"

class LifeLink : public Ability {
public:
  std::string getName() const override {
    return "Life Link";
  }
  
  std::string getDesc() const override {
    return "Damage inflicted is also given to player.";
  }
  
  void usePreAttack(Duel& duel, Creature* blocker) override {
    if(duel.attacker->isAlive()) {
      int damage = std::min(duel.attacker->getPower(), blocker->getToughness());
      duel.player->addHP(damage);
    }
  }
  
  void usePostDuelAttack(Duel& duel) override {
    if(duel.blockers.size() == 0 || duel.attacker->hasAbility<Trample>()) {
      int damage = std::min(duel.attacker->getPower(), duel.adversary->getHP());
      duel.player->addHP(damage);
    }
  }
};
