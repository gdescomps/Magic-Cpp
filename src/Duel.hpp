#pragma once

#include <vector>

class Creature;
class Player;

class Duel {

public:
  Creature* attacker;
  std::vector<Creature*> blockers;
  Player* player;
  Player* adversary;

  Duel(Creature* attacker, std::vector<Creature*> blockers, Player* player, Player* adversary)
    : attacker(attacker), blockers(blockers), player(player), adversary(adversary)
  {}
  
  void performDuel();
};
