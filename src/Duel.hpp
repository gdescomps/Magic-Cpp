#pragma once

#include <vector>

#include "Card.hpp"
#include "Player.hpp"

class Duel {
  

public:
  
Duel(
  Card const* cardAttacker,
  std::vector<Card const*> cardBlockers,
  Player const* playerAttacker
) : cardAttacker(cardAttacker), cardBlockers(cardBlockers), playerAttacker(playerAttacker), 
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

private:
  Card const* cardAttacker;
  std::vector<Card const*> cardBlockers;
  Player const* playerAttacker;
  Player const* playerBlocker;

  int cardAttackerDamage;
  std::vector<int> cardBlockersDamages;
  int playerAttackerDamage;
  int playerBlockerDamage;
};
