#pragma once

#include "../Creature.hpp"

class CrownTheVirulent : public Creature {

public:
  
  CrownTheVirulent(): Creature(3, 3) {}
  
  std::string getName() const override { return "Crown The Virulent"; }
  std::string getType() const override { return Creature::getType() + " - Monster"; }
  std::string getDesc() const override { return "It's tenacity is unbearable."; }
  
  ManaCost getCost() const override {
    ManaCost cost;
    cost.set(Mana::RED, 2);
    return cost;
  }

  Mana getMana() const override {
    return Mana::RED;
  }
};
