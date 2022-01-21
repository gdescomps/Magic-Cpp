#pragma once

#include "../Creature.hpp"

class LeojTheMerciful : public Creature {

public:
  
  LeojTheMerciful(): Creature(5, 1) {}
  
  std::string getName() const override { return "Leoj The Merciful"; }
  std::string getType() const override { return Creature::getType() + " - $5"; }
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
};

