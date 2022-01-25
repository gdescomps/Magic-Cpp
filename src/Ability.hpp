#pragma once

#include <vector>
#include <string>

class Duel;
class Creature;

class Ability {
public:  
  virtual bool validateAttack(Duel const&) { return true; }
  virtual bool validateBlock(Duel const&, Creature*) { return true; }

  virtual void usePreAttack(Duel&) { }
  virtual void usePostAttack(Duel&) { }
  virtual void usePreBlock(Duel&, Creature*) { }
  virtual void usePostBlock(Duel&, Creature*) { }
  virtual void usePreDuel(Duel&, Creature*) { }
  virtual void usePostDuel(Duel&, Creature*) { }

  virtual std::string getName() const = 0;
  virtual std::string getDesc() const { return ""; }
};

template<class... As>
inline std::vector<Ability*> make_abilities() {
  return { (As::getInst())... };
}