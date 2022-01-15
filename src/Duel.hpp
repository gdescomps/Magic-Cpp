#pragma once

#include <vector>

class Creature;
class Player;

class Duel {

public:
  
Duel(
  Creature* cardAttacker,
  std::vector<Creature*> cardBlockers,
  Player* playerAttacker,
  Player* playerBlocker
) : cardAttacker(cardAttacker), cardBlockers(cardBlockers),
    playerAttacker(playerAttacker), playerBlocker(playerBlocker), 
    cardAttackerDamage(0), cardBlockersDamages(cardBlockers.size()), playerBlockerDamage(0)
{}

  int getCardAttackerDamage() const;
  std::vector<int> getCardBlockersDamages() const;
  int getPlayerAttackerDamage() const;
  int getPlayerBlockerDamage() const;

  void setCardAttackerDamage(int damage);
  void setCardBlockersDamages(std::vector<int> damage);
  void setPlayerAttackerDamage(int damage);
  void setPlayerBlockerDamage(int damage);

  Creature* const cardAttacker;
  std::vector<Creature*> const cardBlockers;
  Player* const playerAttacker;
  Player* const playerBlocker;

private:
  int cardAttackerDamage;
  std::vector<int> cardBlockersDamages;
  int playerAttackerDamage;
  int playerBlockerDamage;
};
