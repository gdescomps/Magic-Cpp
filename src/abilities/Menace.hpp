#pragma once

#include "../Ability.hpp"
#include "../Duel.hpp"

class Menace : public Ability {
public:
  bool validateAttack(Duel const& duel) override {
    return duel.blockers.size() != 1;
  }
  
  std::string getName() const override {
    return "Menace";
  }
  
  std::string getDesc() const override {
    return "cannot be blocked by a single creature";
  }
  
  static Menace* getInst() {
    static Menace inst;
    return &inst;
  }
};

