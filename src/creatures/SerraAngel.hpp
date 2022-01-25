#pragma once

#include "../Creature.hpp"
#include "../CardRegistry.hpp"

class SerraAngel : public Creature {

public:

  SerraAngel(): Creature(4, 4) {}

  std::string getName() const override { return "Serra Angel"; }
  std::string getType() const override { return Creature::getType() + " - Angel"; }


  ManaCost getCost() const override {
    ManaCost cost;
    cost.setAny(3);
    cost.set(Mana::WHITE, 2);
    return cost;
  }

  Mana getMana() const override {
    return Mana::WHITE;
  }
  
  int getCardId() const override {
    static int id = CardRegistry::getInst().registerCard<SerraAngel>();
    return id;
  }
};
