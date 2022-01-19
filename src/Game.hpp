#pragma once

#include "Interface.hpp"
#include "Player.hpp"

/** The main class to start and run a game. */
class Game {

public:
Game();

/** Start a new Game. */
void play();

private:
  Interface iface;
  Player player1, player2;

  /** Play one player's turn. */
  bool playTurn(Player* p);
  
  /** String representation of a player's name e.g."Player 1", "Player 2". */
  std::string getPlayerName(Player* p);
    
  /** String representation of an ordinal e.g."1st", "2nd", "3rd"... */
  std::string getOrdinal(int i);
    
  /** Return a pointer to the other player. */
  Player* switchPlayer(Player* last);
  
  /** Game phase where the player draws a card. */
  bool drawPhase(Player* player);
  
  /** Game phase where the player places a creature on battlefield (and tap cards). */
  bool placeCreature(Player* player);

  /** Game phase where the player places a land on battlefield. */
  bool placeLand(Player* player);
  
  /** Game phase where the player attack the adversary. */
  bool attackPhase(Player* player);

  /** Menu to show a player's cards. */
  void menuShowCards(Player* player);
  
  bool hasPlaceableLands(Player* player);
  bool hasAttackers(Player* player);
};

