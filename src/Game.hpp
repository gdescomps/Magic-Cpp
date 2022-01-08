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
  std::vector<Card*> getPlaceables(Player* player);
};

