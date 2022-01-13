#include <memory>
#include <algorithm>

#include "Game.hpp"
#include "creatures/SerraAngel.hpp"
#include "Land.hpp"
#include "Mana.hpp"

std::vector<std::unique_ptr<Card>> makePlayer1Deck() {
  std::vector<std::unique_ptr<Card>> res;
  res.push_back(std::make_unique<Land>(Mana::BLUE));
  res.push_back(std::make_unique<Land>(Mana::BLACK));
  res.push_back(std::make_unique<Land>(Mana::BLUE));
  res.push_back(std::make_unique<Land>(Mana::WHITE));
  res.push_back(std::make_unique<Land>(Mana::WHITE));
  res.push_back(std::make_unique<SerraAngel>());
  res.push_back(std::make_unique<Land>(Mana::RED));
  res.push_back(std::make_unique<Land>(Mana::RED));
  res.push_back(std::make_unique<Land>(Mana::RED));
  return res;
}

std::vector<std::unique_ptr<Card>> makePlayer2Deck() {
  std::vector<std::unique_ptr<Card>> res;
  res.push_back(std::make_unique<Land>(Mana::BLACK));
  res.push_back(std::make_unique<Land>(Mana::BLACK));
  res.push_back(std::make_unique<Land>(Mana::BLACK));
  res.push_back(std::make_unique<Land>(Mana::RED));
  res.push_back(std::make_unique<Land>(Mana::RED));
  res.push_back(std::make_unique<Land>(Mana::RED));
  res.push_back(std::make_unique<Land>(Mana::RED));
  res.push_back(std::make_unique<Land>(Mana::RED));
  return res;
}

int getInt(std::string s) {
  try {
    return std::stoi(s);
  } catch (...) { 
    return -1; 
  }
}


Game::Game()
  : player1(makePlayer1Deck()),
  player2(makePlayer2Deck())
{
  for(size_t i = 0; i < 5; i++) {
    player1.getCards<Card>()[i]->setState(Card::State::BATTLEFIELD);
  }
}

void Game::play() {
  Player* activePlayer = &player1;
  iface.tell(getPlayerName(activePlayer) + " starts playing.");

  while(playTurn(activePlayer)) {
    activePlayer = switchPlayer(activePlayer);
    iface.tell(getPlayerName(activePlayer) + " starts playing.");
  }

  activePlayer = switchPlayer(activePlayer);
  iface.tell("Game has ended, " + getPlayerName(activePlayer) + " wins.");
}

bool Game::drawPhase(Player* player) {
  Card* drawn = player->drawCard();

  if(drawn == nullptr) {
    iface.tell(getPlayerName(player) + " has an empty library.");
    // layer loses
    return false;
  }

  iface.showCard(drawn);
  iface.tell(getPlayerName(player) + " drew a card.");
  iface.hideAll();
  return true;
}

bool Game::placeLand(Player* player) {
  // find placeable lands
  auto lands = player->getCardsInState<Land>(Card::State::HAND);
  
  // ask to place a land
  if(lands.size() != 0) {
    Card* card = iface.selectCard("Place a land?", lands);
    if(card != nullptr) {
      card->setState(Card::State::BATTLEFIELD);
      lands.erase(std::ranges::find(lands, card)); // remove placed card from vec of placeable lands
      return true;
    }
  }

  else {
    iface.tell("No land to place.");
  }

  return false;
}

bool Game::placeCreature(Player* player) {
  // find tappable lands
  auto lands = player->getCardsInState<Land>(Card::State::BATTLEFIELD);
  std::erase_if(lands, [] (Card* c) { return c->isTapped(); });

  ManaCost cost;

  // ask lands to tap and build ManaCost
  if(lands.size() != 0) {
    auto selected = iface.selectCards("Tap lands?", lands);
    for(Land* land : selected) {
      cost.add(land->getMana());
    }
  }
  else {
    iface.tell("No tappable land.");
  }

  // find placeable cards (except lands)
  auto placeables = player->getPlaceableCards<Card>(cost); 
  std::erase_if(placeables, [] (Card* c) {
    return dynamic_cast<Land*>(c) != nullptr;
  });  
  
  // ask to place a card
  if(placeables.size() != 0) {
    auto selected = iface.selectCards("Use cards?", placeables);
    for(Card* card : selected) {
      card->tap();            
      card->setState(Card::State::BATTLEFIELD);
      return true;
    }
  }
  else {
    iface.tell("No card to place with the tapped mana.");
  }

  return false;
}

bool Game::attackPhase(Player* player) {
  // find creatures usable in attack
  auto creatures = player->getCardsInState<Creature>(Card::State::BATTLEFIELD);
  std::erase_if(creatures, [] (Card* c) { return c->isTapped(); });
  
  // ask creatures to use
  if(creatures.size() != 0) {
    std::vector<Creature*> used = iface.selectCards<Creature>("Select creatures to use in attack", creatures);
  }  
  else {
    iface.tell("No creature to use");
  }

  Player* adversary = switchPlayer(player);
    
  return true;
}

void Game::menuShowCards(Player* player) {
  int choice = iface.showMenu("What to show?", {
    "Show hand",
    "Show battlefield",
    "Go back",
  });
  
  if(choice == 0) { // show player hand
    auto msg = getPlayerName(player) + "'s hand";
    auto cards = player->getCardsInState<Card>(Card::State::HAND);
    iface.showCards(msg, cards);
  }
  
  else if(choice == 1) { // show battlefield 
    auto msg = getPlayerName(player) + "'s cards on the battlefield";
    auto cards = player->getCardsInState<Card>(Card::State::BATTLEFIELD);
    iface.showCards(msg, cards);
  }
}
bool Game::playTurn(Player* player) {
  using MenuEntry::State::NORMAL;
  using MenuEntry::State::DISABLED;

  bool cont = drawPhase(player);
  player->untapAll();
  
  bool hasAttacked = false;
  bool canPlaceLand = hasPlaceableLands(player);
  bool canAttack = hasPlaceableCreatures(player);
  
  while(cont) {
    
    int choice = iface.showMenu("What to do?", {
      "Show...",
      MenuEntry("Place a Land", canPlaceLand ? NORMAL : DISABLED),
      "Place a Creature",
      MenuEntry("Attack", canAttack ? NORMAL : DISABLED),
      "Finish turn",
    });
  
    if(choice == 0) { // show submenu
      menuShowCards(player);
    }
    else if(choice == 1) { // place a land
      canPlaceLand = placeLand(player);
      canAttack = !hasAttacked && hasPlaceableCreatures(player);
    }
    else if(choice == 2) { // place cards
      placeCreature(player);
      canAttack = !hasAttacked && hasPlaceableCreatures(player);
    }
    else if(choice == 3) { // attack
      hasAttacked = attackPhase(player);
    }
    else if(choice == 4) { // finish turn
      return cont;      
    }
  }

  return cont;
}

bool Game::hasPlaceableLands(Player* player) {
  return player->getCardsInState<Land>(Card::State::HAND).size() != 0;
}

bool Game::hasPlaceableCreatures(Player* player) {
  // TODO: tell if enough mana
  return player->getCardsInState<Creature>(Card::State::HAND).size() != 0;
}

std::string Game::getPlayerName(Player* p) {
  return p == &player1 ? "Player 1" : "Player 2";
}

Player* Game::switchPlayer(Player* last) {
  return last == &player1 ? &player2 : &player1;
}
