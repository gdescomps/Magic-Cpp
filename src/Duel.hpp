#pragma once

#include <vector>
#include <string>

class Creature;
class Player;
class Ability;

struct DuelValidation {
  operator bool();
  
  DuelValidation(bool ok, std::string msg);
  static DuelValidation ok();
  static DuelValidation ko(std::string msg);
  static DuelValidation ko(Ability* a, Creature* c);

  std::string getMsg();
  
private:
  bool isOk;
  std::string msg;
};

class Duel {

public:
  Creature* attacker;
  std::vector<Creature*> blockers;
  Player* player;
  Player* adversary;

  Duel(Creature* attacker, std::vector<Creature*> blockers, Player* player, Player* adversary)
    : attacker(attacker), blockers(blockers), player(player), adversary(adversary)
  {}
  
  DuelValidation validateDuel();
  void performDuel();
};

