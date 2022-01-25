#pragma once

#include "../Creature.hpp"
#include "../CardRegistry.hpp"

class LeojTheMerciful : public Creature {

public:
  
  LeojTheMerciful(): Creature(5, 1) {}
  
  std::string getName() const override { return "Leoj The Merciful"; }
  std::string getType() const override { return Creature::getType() + " - Grey Knight"; }
  std::string getDesc() const override { return "Don't be fooled by the name, it will destroy you in an instant."; }
  
  ManaCost getCost() const override {
    ManaCost cost;
    cost.set(Mana::WHITE, 1);
    cost.set(Mana::BLACK, 1);
    return cost;
  }

  Mana getMana() const override {
    return Mana::BLACK;
  }

  int getCardId() const override {
    static int id = CardRegistry::getInst().registerCard<LeojTheMerciful>();
    return id;
  }
};
