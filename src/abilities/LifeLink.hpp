#pragma once

#include "../Ability.hpp"

class LifeLink : public Ability {
public:
  std::string getName() const override {
    return "Life Link";
  }
  
  std::string getDesc() const override {
    return "Damage inflicted is also given to player.";
  }
  
  static LifeLink* getInst() {
    static LifeLink inst;
    return &inst;
  }
};
