#pragma once

#include "../Creature.hpp"
#include "../abilities/ColorImmunity.hpp"

class RatMan : public Creature {

public:
  
  RatMan(): Creature(2, 2, make_abilities<
    ColorImmunity<Mana::BLUE>,
    ColorImmunity<Mana::GREEN>,
    ColorImmunity<Mana::RED>,
    ColorImmunity<Mana::WHITE>
  >()) {}
  
  std::string getName() const override { return "RatMan"; }
  std::string getType() const override { return Creature::getType() + " - Horror"; }
  std::string getDesc() const override { return "Disgusting."; }

  ManaCost getCost() const override {
    ManaCost cost;
    cost.set(Mana::BLACK, 1);
    cost.setAny(1);
    return cost;
  }

  Mana getMana() const override {
    return Mana::BLACK;
  }
};

