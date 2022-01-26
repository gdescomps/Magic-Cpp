#pragma once

#include "../Ability.hpp"
#include "../Duel.hpp"

class Haste : public Ability {
public:
  std::string getName() const override {
    return "Haste";
  }
  
  std::string getDesc() const override {
    return "Can be used directly when placed.";
  }
};
