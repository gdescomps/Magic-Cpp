#pragma once

#include "../Creature.hpp"

#include "../abilities/LifeLink.hpp"
#include "../abilities/DeathTouch.hpp"
#include "../abilities/Flying.hpp"
#include "../abilities/ColorImmunity.hpp"
#include "../abilities/Haste.hpp"
#include "../abilities/Menace.hpp"
#include "../abilities/Reach.hpp"
#include "../abilities/Trample.hpp"
#include "../abilities/Unblockable.hpp"
#include "../abilities/Vigilance.hpp"
 

class Thanos : public Creature {

public:
  
  Thanos(): Creature(3, 3, make_abilities<
    LifeLink,
    DeathTouch,
    Flying,
    ColorImmunity<Mana::RED>,
    Haste,
    Menace,
    Reach,
    Trample,
    Unblockable,
    Vigilance>()) {}
  
  std::string getName() const override { return "Thanos"; }
  std::string getType() const override { return Creature::getType() + " - Warrior"; }
  std::string getDesc() const override { return "Snap!"; }

  ManaCost getCost() const override {
    ManaCost cost;
    cost.add(Mana::BLACK);
    cost.add(Mana::BLUE);
    cost.add(Mana::GREEN);
    cost.add(Mana::RED);
    cost.add(Mana::WHITE);
    return cost;
  }

  Mana getMana() const override {
    return Mana::RED;
  }
};

