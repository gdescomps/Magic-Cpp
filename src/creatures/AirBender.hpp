#pragma once

#include "../Creature.hpp"
#include "../CardRegistry.hpp"

class AirBender : public Creature {

public:
  
  AirBender(): Creature(1, 5) {}
  
  std::string getName() const override { return "Air Bender"; }
  std::string getType() const override { return Creature::getType() + " - Witch"; }
  std::string getDesc() const override { return "Oxygen is highly inflammable."; }
  
  ManaCost getCost() const override {
    ManaCost cost;
    cost.set(Mana::WHITE, 1);
    cost.set(Mana::BLUE, 1);
    return cost;
  }

  Mana getMana() const override {
    return Mana::BLUE;
  }
  
  int getCardId() const override {
    static int id = CardRegistry::getInst().registerCard<AirBender>();
    return id;
  }
};
