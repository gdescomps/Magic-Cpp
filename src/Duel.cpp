#include "Duel.hpp"
#include "Creature.hpp"
#include "Player.hpp"
#include "Ability.hpp"

DuelValidation Duel::validateDuel() {
  auto res = attacker->validateAttack(*this);
  if(!res) return res;

  for(auto blocker : blockers) {
    res = blocker->validateBlock(*this);
    if(!res) return res;
  }

  return DuelValidation::ok();
}

void Duel::performAttack(Creature* attacker, Creature* blocker) {
  int attackerToughness = attacker->getToughness();
  int attackerPower = attacker->getPower();
  int blockerToughness = blocker->getToughness();
  int blockerPower = blocker->getPower();
  
  // perform the attack simultaneously on the attacker & blocker
  attacker->setToughness(std::max(0, attackerToughness - blockerPower));
  attacker->setPower(std::max(0, attackerPower - blockerToughness));
  blocker->setToughness(std::max(0, blockerToughness - attackerPower));
  blocker->setPower(std::max(0, blockerPower - attackerToughness));
}

void Duel::performDuel() {
  // pre duel hooks
  for(auto a : attacker->getAbilities()) a->usePreDuel(*this, attacker);
  for(auto b : blockers) for(auto a : b->getAbilities()) a->usePreDuel(*this, b);

  for(Creature* blocker : blockers) {
    // pre attack hooks
    for(auto a : attacker->getAbilities()) a->usePreAttack(*this);
    for(auto a : blocker->getAbilities()) a->usePreBlock(*this, blocker);

    performAttack(attacker, blocker);

    // post attack hooks
    for(auto a : attacker->getAbilities()) a->usePostAttack(*this);
    for(auto a : blocker->getAbilities()) a->usePostBlock(*this, blocker);
  }
  
  // deal excess damage to the adversary
  adversary->setHP(std::max(0, adversary->getHP() - attacker->getPower()));
  
  // post duel hooks
  for(auto a : attacker->getAbilities()) a->usePostDuel(*this, attacker);
  for(auto b : blockers) for(auto a : b->getAbilities()) a->usePreDuel(*this, b);
}

DuelValidation::operator bool() { return isOk; }
DuelValidation::DuelValidation(bool ok, std::string msg) : isOk(ok), msg(msg) {}
DuelValidation DuelValidation::ok() { return DuelValidation(true, "ok"); }
DuelValidation DuelValidation::ko(std::string msg) { return DuelValidation(false, msg); }
DuelValidation DuelValidation::ko(Ability* a, Creature* c) { return DuelValidation(false, c->getName() + " has ability " + a->getName()); }
std::string DuelValidation::getMsg() { return msg; }
