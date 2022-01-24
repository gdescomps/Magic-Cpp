#pragma once

#include <vector>
#include <string>

class Duel;
class Creature;

class Ability {
public:  
  virtual bool validateAttack(Duel const&) { return true; }
  virtual bool validateBlock(Duel const&, Creature*) { return true; }
  virtual std::string getName() const = 0;
  virtual std::string getDesc() const { return ""; }
};

template<class... As>
inline std::vector<Ability*> make_abilities() {
  return { (As::getInst())... };
}