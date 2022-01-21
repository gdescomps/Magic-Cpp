#pragma once

#include "../Creature.hpp"

class $1 : public Creature {

public:
  
  $1(): Creature($2, $3) {}
  
  std::string getName() const override { return "$4"; }
  std::string getType() const override { return Creature::getType() + " - $5"; }
  std::string getDesc() const override { return "$6"; }
  
  ManaCost getCost() const override {
    ManaCost cost;
    return cost;
  }

  Mana getMana() const override {
    return Mana::WHITE;
  }
};
