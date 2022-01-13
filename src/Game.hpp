#pragma once

#include "Interface.hpp"
#include "Player.hpp"

class Game {

public:
Game();

void play();

private:
  Interface iface;
  bool playTurn(Player* p);
  Player player1, player2;

  std::string getPlayerName(Player* p);
  Player* switchPlayer(Player* last);
  
  bool drawPhase(Player* player);
  bool placeCreature(Player* player);
  bool placeLand(Player* player);
  bool attackPhase(Player* player);
  void menuShowCards(Player* player);
  
  bool hasPlaceableLands(Player* player);
  bool hasPlaceableCreatures(Player* player);
};

