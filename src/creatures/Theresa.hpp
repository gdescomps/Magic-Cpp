#pragma once

#include "../Creature.hpp"
#include "../CardRegistry.hpp"

class Theresa : public Creature {

public:

  Theresa(): Creature(0, 10) {}

  std::string getName() const override { return "Theresa"; }
  std::string getType() const override { return Creature::getType() + " - Ent"; }
  std::string getDesc() const override { return "A large oak tree."; }

  ManaCost getCost() const override {
    ManaCost cost;
    cost.setAny(1);
    cost.set(Mana::GREEN, 3);
    return cost;
  }

  Mana getMana() const override {
    return Mana::GREEN;
  }
};
