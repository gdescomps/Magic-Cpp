#pragma once

#include <vector>

class Creature;
class Player;

class Duel {

private:
  Creature* cardAttacker;
  std::vector<Creature*> cardBlockers;
  Player* playerAttacker;
  Player* playerBlocker;

public:

  Duel(Creature* cardAttacker, std::vector<Creature*> cardBlockers, Player* playerAttacker, Player* playerBlocker)
    : cardAttacker(cardAttacker), cardBlockers(cardBlockers), playerAttacker(playerAttacker), playerBlocker(playerBlocker)
  {}

  Creature* getCardAttacker() {return cardAttacker;}
  std::vector<Creature*> getCardBlockers() {return cardBlockers;}

};
