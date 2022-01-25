#pragma once

#include "../Ability.hpp"

class Defender : public Ability {
public:
  
  bool validateAttack(Duel const&) override {
    return false;
  }
  
  std::string getName() const override {
    return "Defender";
  }
  
  std::string getDesc() const override {
    return "Cannot attack.";
  }
  
  static Defender* getInst() {
    static Defender inst;
    return &inst;
  }
};



