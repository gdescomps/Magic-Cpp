#pragma once

#include "../Creature.hpp"

class RatMan : public Creature {

public:
  
  RatMan(): Creature(6, 2) {}
  
  std::string getName() const override { return "RatMan"; }
  std::string getType() const override { return Creature::getType() + " - Horror"; }
  std::string getDesc() const override { return "Disgusting."; }

  ManaCost getCost() const override {
    ManaCost cost;
    return cost;
  }

  Mana getMana() const override {
    return Mana::BLACK;
  }
};

