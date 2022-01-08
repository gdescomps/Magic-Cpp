#include <memory>
#include <algorithm>

#include "Game.hpp"
#include "creatures/SerraAngel.hpp"
#include "Land.hpp"
#include "Mana.hpp"

std::vector<std::unique_ptr<Card>> makePlayer1Deck() {
  std::vector<std::unique_ptr<Card>> res;
  res.push_back(std::make_unique<Land>(Mana::BLUE));
  res.push_back(std::make_unique<SerraAngel>());
  return res;
}

std::vector<std::unique_ptr<Card>> makePlayer2Deck() {
  std::vector<std::unique_ptr<Card>> res;
  res.push_back(std::make_unique<Land>(Mana::BLACK));
  res.push_back(std::make_unique<Land>(Mana::BLACK));
  res.push_back(std::make_unique<Land>(Mana::BLACK));
  return res;
} 

Game::Game()
  : player1(makePlayer1Deck()),
  player2(makePlayer2Deck())
{}

void Game::play() {
  iface.showWelcome();

  Player* activePlayer = &player1;

  while(playTurn(activePlayer)) {
    activePlayer = switchPlayer(activePlayer);
  }

  iface.prompt("Game has ended, " + getPlayerName(activePlayer) + " wins.");
}

bool Game::playTurn(Player* player) {

  // draw phase
  Card* drawn = player->drawCard();

  if(drawn == nullptr) {
    // layer loses
    return false;
  }

  iface.showCard(drawn);
  iface.prompt("Press Enter");
  iface.hideAll();

  // untap phase
    
  player->untapAll();
  
  // place phase
  
  auto hand = player->getCardsInState(Card::State::HAND);
  ManaCost cost;
  auto placeables = player->getPlaceableCards(cost); 
  iface.showCards(placeables);
  iface.prompt("Choice? (1-" + std::to_string(placeables.size()) + ")");
  iface.hideAll();

  // attack phase

  // 2nd. place phase

  return true;
}

std::string Game::getPlayerName(Player* p) {
  return p == &player1 ? "Player 1" : "Player 2";
}

Player* Game::switchPlayer(Player* last) {
  return last == &player1 ? &player2 : &player1;
}

