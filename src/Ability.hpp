#pragma once

#include <vector>
#include <string>

class Duel;
class Creature;

class Ability {
public:  
  virtual bool validateAttack(Duel const&) { return true; }
  virtual bool validateBlock(Duel const&, Creature*) { return true; }
  
  // for below methods, the Creature* param is the blocker.
  virtual void usePreAttack(Duel&, Creature* /* blocker */) { }
  virtual void usePostAttack(Duel&, Creature* /* blocker */) { }
  virtual void usePreBlock(Duel&, Creature* /* blocker */) { }
  virtual void usePostBlock(Duel&, Creature* /* blocker */) { }
  virtual void usePreDuel(Duel&, Creature* /* blocker */) { }
  virtual void usePostDuel(Duel&, Creature* /* blocker */) { }

  virtual std::string getName() const = 0;
  virtual std::string getDesc() const { return ""; }
};

template<class... As>
inline std::vector<Ability*> make_abilities() {
  return { (As::getInst())... };
}