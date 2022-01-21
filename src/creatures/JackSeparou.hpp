#pragma once

#include "../Creature.hpp"

class JackSeparou : public Creature {

public:
  
  JackSeparou(): Creature(3, 2) {}
  
  std::string getName() const override { return "Jack Separou"; }
  std::string getType() const override { return Creature::getType() + " - Pirate"; }
  std::string getDesc() const override { return "A forgetful remake."; } 
  
  ManaCost getCost() const override {
    ManaCost cost;
    cost.setAny(1);
    cost.set(Mana::BLACK, 1);
    cost.set(Mana::BLUE, 1);
    return cost;
  }

  Mana getMana() const override {
    return Mana::BLUE;
  }
};

