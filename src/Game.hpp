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
  bool placePhase(Player* player);
  bool attackPhase(Player* player);
};

