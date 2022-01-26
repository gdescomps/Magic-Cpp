#pragma once

#include <vector>
#include <string>
#include <memory>

class Duel;
class Creature;

class Ability {
public:
  virtual ~Ability() = default;

  virtual bool validateAttack(Duel const&) { return true; }
  virtual bool validateBlock(Duel const&, Creature*) { return true; }
  
  // for below methods, the Creature* param is the blocker.
  virtual void usePreAttack(Duel&, Creature* /* blocker */) { }
  virtual void usePostAttack(Duel&, Creature* /* blocker */) { }
  virtual void usePreBlock(Duel&, Creature* /* blocker */) { }
  virtual void usePostBlock(Duel&, Creature* /* blocker */) { }
  virtual void usePreDuelAttack(Duel&) { }
  virtual void usePostDuelAttack(Duel&) { }
  virtual void usePreDuelBlock(Duel&, Creature* /* blocker */) { }
  virtual void usePostDuelBlock(Duel&, Creature* /* blocker */) { }

  virtual std::string getName() const = 0;
  virtual std::string getDesc() const { return ""; }
};

template<class... As>
inline std::vector<std::unique_ptr<Ability>> make_abilities() {
  std::vector abilities = { ((Ability*)new As())... };
  std::vector<std::unique_ptr<Ability>> res(abilities.size());
  for(size_t i = 0; i < abilities.size(); i++) {
    res[i] = std::unique_ptr<Ability>(abilities[i]);
  }
  return res;
}