#pragma once

#include "../Ability.hpp"
#include "../Duel.hpp"

class Vigilance : public Ability {
public:
  std::string getName() const override {
    return "Vigilance";
  }
  
  std::string getDesc() const override {
    return "Attacking does not cause the creature to tap.";
  }
};



