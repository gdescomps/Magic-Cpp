#include <memory>
#include <algorithm>

#include "Game.hpp"
#include "creatures/SerraAngel.hpp"
#include "Land.hpp"
#include "Mana.hpp"
#include "Duel.hpp"

std::vector<std::unique_ptr<Card>> makePlayer1Deck() {
  std::vector<std::unique_ptr<Card>> res;
  res.push_back(std::make_unique<SerraAngel>());
  res.push_back(std::make_unique<SerraAngel>());
  res.push_back(std::make_unique<SerraAngel>());
  res.push_back(std::make_unique<SerraAngel>());
  res.push_back(std::make_unique<SerraAngel>());
  res.push_back(std::make_unique<Land>(Mana::BLUE));
  res.push_back(std::make_unique<Land>(Mana::BLACK));
  res.push_back(std::make_unique<Land>(Mana::BLUE));
  res.push_back(std::make_unique<Land>(Mana::WHITE));
  res.push_back(std::make_unique<Land>(Mana::WHITE));
  res.push_back(std::make_unique<Land>(Mana::RED));
  res.push_back(std::make_unique<Land>(Mana::RED));
  res.push_back(std::make_unique<Land>(Mana::RED));
  return res;
}

std::vector<std::unique_ptr<Card>> makePlayer2Deck() {
  std::vector<std::unique_ptr<Card>> res;
  res.push_back(std::make_unique<SerraAngel>());
  res.push_back(std::make_unique<SerraAngel>());
  res.push_back(std::make_unique<SerraAngel>());
  res.push_back(std::make_unique<SerraAngel>());
  res.push_back(std::make_unique<SerraAngel>());
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

  if(lands.size() == 0) {
    iface.tell("No tappable land.");
    return false;
  }

  ManaCost cost;

  // ask lands to tap and build ManaCost
  auto selectedLands = iface.selectCards("Tap lands?", lands);
  for(Land* land : selectedLands) {
    cost.add(land->getMana());
  }

  // find placeable cards (except lands)
  auto placeables = player->getPlaceableCards<Card>(cost); 
  std::erase_if(placeables, [] (Card* c) {
    return dynamic_cast<Land*>(c) != nullptr;
  });  
  
  if(placeables.size() == 0) {
    iface.tell("No card to place with the tapped mana.");
  }

  // ask to place a card
  auto selectedCards = iface.selectCards("Use cards?", placeables);
  for(Card* card : selectedCards) {
    card->tap();            
    card->setState(Card::State::BATTLEFIELD);
    return true;
  }

  return false;
}

bool Game::attackPhase(Player* player) {
  std::vector<std::pair<Creature*, std::vector<Creature*>>> duels;  
  
  // find creatures usable in attack
  auto availAttackers = player->getCardsInState<Creature>(Card::State::BATTLEFIELD);
  std::erase_if(availAttackers, [] (Card* c) { return c->isTapped(); });

  if(availAttackers.size() == 0) {
    iface.tell("No creature to use in attack");
    return false;
  }

  // ask creatures to use
  std::vector<Creature*> attackers = iface.selectCards<Creature>("Select creatures to use in attack", availAttackers);

  // find creatures usable as blockers
  Player* adversary = switchPlayer(player);
  auto availBlockers = adversary->getCardsInState<Creature>(Card::State::BATTLEFIELD);
  std::erase_if(availBlockers, [] (Card* c) { return c->isTapped(); });

  // ask adversary for blockers
  while(attackers.size() != 0 && availBlockers.size() != 0) {

    // choose an attacker
    Creature* attacker = iface.selectCard(getPlayerName(adversary) + ", choose a card to block", attackers);
    if(attacker == nullptr) {
      bool resp = iface.promptYesNo("Finished choosing blockers?");
      if(resp) break;
    }
    
    // choose blockers
    auto blockers = iface.selectCards("Choose cards to block this attacker", availBlockers);
    
    // create the duel and remove attacker / blockers
    if(blockers.size() != 0) {
      duels.emplace_back(attacker, blockers);
      attackers.erase(std::ranges::find(attackers, attacker));
      for(auto blocker : blockers) {
        availBlockers.erase(std::ranges::find(availBlockers, blocker));
      }
    }
    
    // ask player to order attacks for duels with multiple blockers
    for(auto& duel : duels) if(duel.second.size() >= 2) {
      for(size_t i = 0; i < duel.second.size() - 1; i++) {
        Card* card;
        do card = iface.selectCard("Choose the " + getOrdinal(i) + " card to attack with your " + duel.first->getName(), duel.second);
        while(card == nullptr);
        std::iter_swap(duel.second.begin() + i, std::ranges::find(duel.second, card));
      }
    }
  }
  
  // TODO Perform the DUELS
  
  return true;
}

void Game::menuShowCards(Player* player) {
  int choice = iface.showMenu("What to show?", {
    "Show your hand",
    "Show your battlefield",
    "Show adversary battlefield",
    "Go back",
  });
  
  if(choice == 0) { // show player hand
    auto msg = getPlayerName(player) + "'s hand";
    auto cards = player->getCardsInState<Card>(Card::State::HAND);

    if(cards.size() > 0)
      iface.showCards(msg, cards);
    else 
      iface.tell("Hand is empty");
  }
  
  else if(choice == 1) { // show battlefield 
    auto msg = getPlayerName(player) + "'s cards on the battlefield";
    auto cards = player->getCardsInState<Card>(Card::State::BATTLEFIELD);

    if(cards.size() > 0)
      iface.showCards(msg, cards);
    else 
      iface.tell("Battlefield is empty");
  }
  
  else if(choice == 2) { // show adversary battlefield 
    auto adversary = switchPlayer(player);
    auto msg = getPlayerName(adversary) + "'s cards on the battlefield";
    auto cards = adversary->getCardsInState<Card>(Card::State::BATTLEFIELD);

    if(cards.size() > 0)
      iface.showCards(msg, cards);
    else 
      iface.tell("Battlefield is empty");
  }
}
bool Game::playTurn(Player* player) {
  using MenuEntry::State::NORMAL;
  using MenuEntry::State::DISABLED;

  bool cont = drawPhase(player);
  player->untapAll();

  auto hasAttackCreatures = [&] () {
    auto creatures = player->getCardsInState<Creature>(Card::State::BATTLEFIELD);
    auto isUntapped = [] (auto* card) { return !card->isTapped(); };
    return std::ranges::find_if(creatures, isUntapped) != creatures.end();
  };
  
  bool hasAttacked = false;
  bool canPlaceLand = player->getCardsInState<Land>(Card::State::HAND).size() != 0;
  bool canAttack = hasAttackCreatures();
  
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
      canAttack = !hasAttacked && hasAttackCreatures();
    }
    else if(choice == 2) { // place cards
      placeCreature(player);
      canAttack = !hasAttacked && hasAttackCreatures();
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

std::string Game::getPlayerName(Player* p) {
  return p == &player1 ? "Player 1" : "Player 2";
}

std::string Game::getOrdinal(int i) {
  if(i == 1) return "1st";
  else if(i == 2) return "2nd";
  else if(i == 3) return "3rd";
  else return std::to_string(i) + "th";
}

Player* Game::switchPlayer(Player* last) {
  return last == &player1 ? &player2 : &player1;
}
