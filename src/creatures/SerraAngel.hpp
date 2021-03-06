#pragma once

#include "../Creature.hpp"
#include "../abilities/Flying.hpp"
#include "../abilities/Vigilance.hpp"

class SerraAngel : public Creature {

public:

  SerraAngel(): Creature(4, 4, make_abilities<Flying, Vigilance>()) 
  {}

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
};
