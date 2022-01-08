#pragma once

#include "../Creature.hpp"

class SerraAngel : public Creature {

public:

  std::string getName() const override { return "Serra Angel"; }
  std::string getType() const override { return Creature::getType() + " - Angel"; }
  
  int getPower() const override { return 4; }
  int getToughness() const override { return 4; }
  
  ManaCost getCost() const override {
    ManaCost cost;
    cost.setAny(3);
    cost.set(Mana::WHITE, 2);
    return cost;
  }
private:

};

