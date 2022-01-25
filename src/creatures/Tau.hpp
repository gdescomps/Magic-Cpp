#pragma once

#include "../Creature.hpp"
#include "../abilities/Trample.hpp"
#include "../abilities/Haste.hpp"
#include "../abilities/DeathTouch.hpp"

class Tau : public Creature {

public:
  
  Tau(): Creature(3, 6, make_abilities<Trample, Haste, DeathTouch>()) {}
  
  std::string getName() const override { return "Tau"; }
  std::string getType() const override { return Creature::getType() + " - Abstract"; }
  std::string getDesc() const override { return "Two times better."; }

  ManaCost getCost() const override {
    ManaCost cost;
    cost.set(Mana::BLACK, 2);
    cost.setAny(2);
    return cost;
  }

  Mana getMana() const override {
    return Mana::BLACK;
  }
};
