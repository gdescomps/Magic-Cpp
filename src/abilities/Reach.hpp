#pragma once

#include "../Ability.hpp"

class Reach : public Ability {
public:
  std::string getName() const override {
    return "Reach";
  }
  
  std::string getDesc() const override {
    return "Can block Flying creatures.";
  }
  
  static Reach* getInst() {
    static Reach inst;
    return &inst;
  }
};


