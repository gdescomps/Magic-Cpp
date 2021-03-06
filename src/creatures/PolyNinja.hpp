#pragma once

#include "../Creature.hpp"
#include "../abilities/Haste.hpp"
#include "../abilities/LifeLink.hpp"

class PolyNinja : public Creature {

public:
  
  PolyNinja(): Creature(2, 2, make_abilities<Haste, LifeLink>()) {}
  
  std::string getName() const override { return "Poly-Ninja"; }
  std::string getType() const override { return Creature::getType() + " - Ninja"; }
  
  ManaCost getCost() const override {
    ManaCost cost;
    cost.setAny(1);
    cost.set(Mana::BLUE, 1);
    return cost;
  }

  Mana getMana() const override {
    return Mana::BLUE;
  }
};