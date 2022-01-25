#pragma once

#include "../Creature.hpp"
#include "../CardRegistry.hpp"

class BrandonSoupSirup : public Creature {

public:
  
  BrandonSoupSirup(): Creature(0, 1) {}
  
  std::string getName() const override { return "Brandon SoupSirup"; }
  std::string getType() const override { return Creature::getType() + " - Prophete"; }
  std::string getDesc() const override { return "Brandon's nerd skills aren't transferable to the battlefield."; }
  
  ManaCost getCost() const override {
    ManaCost cost;
    cost.setAny(1);
    return cost;
  }

  Mana getMana() const override {
    return Mana::GREEN;
  }

  int getCardId() const override {
    static int id = CardRegistry::getInst().registerCard<BrandonSoupSirup>();
    return id;
  }
};

