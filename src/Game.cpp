#include <memory>
#include <algorithm>
#include <numeric>
#include <random>

#include "Game.hpp"
#include "Land.hpp"
#include "Mana.hpp"
#include "Duel.hpp"

#include "creatures/AirBender.hpp"
#include "creatures/BrandonSoupSirup.hpp"
#include "creatures/CrownTheVirulent.hpp"
#include "creatures/JackSeparou.hpp"
#include "creatures/LeojTheMerciful.hpp"
#include "creatures/PolyNinja.hpp"
#include "creatures/RatMan.hpp"
#include "creatures/SerraAngel.hpp"
#include "creatures/Theresa.hpp"

void shuffleDeck(Cards& deck) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::ranges::shuffle(deck, gen);
}

Player* randomPlayer(Player* p1, Player* p2) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, 1);
  return distrib(gen) ? p1 : p2;
}

std::vector<std::unique_ptr<Card>> makePlayer1Deck() {
  std::vector<std::unique_ptr<Card>> res;
  res.push_back(std::make_unique<AirBender>());
  res.push_back(std::make_unique<BrandonSoupSirup>());
  res.push_back(std::make_unique<CrownTheVirulent>());
  res.push_back(std::make_unique<JackSeparou>());
  res.push_back(std::make_unique<LeojTheMerciful>());
  res.push_back(std::make_unique<PolyNinja>());
  res.push_back(std::make_unique<RatMan>());
  res.push_back(std::make_unique<SerraAngel>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Land>(Mana::BLACK));
  res.push_back(std::make_unique<Land>(Mana::BLACK));
  res.push_back(std::make_unique<Land>(Mana::BLACK));
  res.push_back(std::make_unique<Land>(Mana::BLUE));
  res.push_back(std::make_unique<Land>(Mana::BLUE));
  res.push_back(std::make_unique<Land>(Mana::BLUE));
  res.push_back(std::make_unique<Land>(Mana::GREEN));
  res.push_back(std::make_unique<Land>(Mana::GREEN));
  res.push_back(std::make_unique<Land>(Mana::GREEN));
  res.push_back(std::make_unique<Land>(Mana::RED));
  res.push_back(std::make_unique<Land>(Mana::RED));
  res.push_back(std::make_unique<Land>(Mana::RED));
  res.push_back(std::make_unique<Land>(Mana::WHITE));
  res.push_back(std::make_unique<Land>(Mana::WHITE));
  res.push_back(std::make_unique<Land>(Mana::WHITE));
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Theresa>());
  // shuffleDeck(res);
  return res;
}

std::vector<std::unique_ptr<Card>> makePlayer2Deck() {
  return makePlayer1Deck();
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
  // TODO: this is temporary
  for(Land* land : player1.getCards<Land>()) {
    land->setState(Card::State::BATTLEFIELD);
  }
  for(Land* land : player2.getCards<Land>()) {
    land->setState(Card::State::BATTLEFIELD);
  }
  for(size_t i = 0; i < 4; i++) {
    player1.getCardsInState<Creature>(Card::State::LIBRARY).at(0)->setState(Card::State::BATTLEFIELD);
    player2.getCardsInState<Creature>(Card::State::LIBRARY).at(0)->setState(Card::State::BATTLEFIELD);
  }
}

void Game::play() {
  
  // random starting player
  Player* player = randomPlayer(&player1, &player2);
  Player* adversary = switchPlayer(player);
  
  // draw cards
  for(size_t i = 0; i < 6; i++) { // 1 less card for starting player
    player->drawCard();
  }
  for(size_t i = 0; i < 7; i++) {
    adversary->drawCard();
  }

  iface.tell(getPlayerName(player) + " starts playing.");

  while(playTurn(player)) {
    player = switchPlayer(player);
    iface.tell(getPlayerName(player) + " starts playing.");
  }

  player = switchPlayer(player);
  iface.tell("Game has ended, " + getPlayerName(player) + " wins.");
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
    else {
      iface.tell("No land was placed.");
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
    return false;
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
  Player* adversary = switchPlayer(player);
  auto p1Name = getPlayerName(player);
  auto p2Name = getPlayerName(adversary);
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
  auto availBlockers = adversary->getCardsInState<Creature>(Card::State::BATTLEFIELD);
  std::erase_if(availBlockers, [] (Card* c) { return c->isTapped(); });

  // ask adversary for blockers
  while(attackers.size() != 0 && availBlockers.size() != 0) {

    // choose an attacker
    Creature* attacker = iface.selectCard(p2Name + ", choose a card to block", attackers);
    if(attacker == nullptr) {
      bool resp = iface.promptYesNo("Finished choosing blockers?");
      if(resp) break;
      else continue;
    }

    // choose blockers
    auto blockers = iface.selectCards("Choose cards to block this attacker", availBlockers);

    // create the duel and remove attacker / blockers
    duels.emplace_back(attacker, blockers);
    if(blockers.size() != 0) {
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
  
  // add remaining attackers
  for(Creature* attacker : attackers) {
    duels.emplace_back(attacker, std::vector<Creature*>{});
  }

  // Perform the DUELS
  for(auto& d : duels) {
    Duel duel(d.first, d.second, player, adversary);
    duel.performDuel();
    
    // Move dead blockers to Graveyard
    std::vector<Creature*> deadBlockers = duel.blockers;
    std::erase_if(deadBlockers, [] (Creature* blocker) {
      return !(blocker->getToughness() == 0);
    });

    if(deadBlockers.size() > 0) {
      std::string deadNames = deadBlockers[0]->getName();
      for(auto it = ++deadBlockers.begin(); it != deadBlockers.end(); ++it) {
        deadNames += ", " + (*it)->getName();
      }

      iface.tell(p2Name + " lost it's " + duel.attacker->getName() + " against " + p1Name +"'s attacker !");
      for(auto blocker : deadBlockers)
        blocker->setState(Card::State::GRAVEYARD);
    }

    // Move dead attacker to Graveyard
    if(duel.attacker->getToughness() == 0) {
      iface.tell(p1Name + " lost it's " + duel.attacker->getName() + " against " + p2Name +"'s blocker(s) !");
      duel.attacker->setState(Card::State::GRAVEYARD);
    }
  }
    
  return true;
}

void Game::menuShowCards(Player* player) {
  int choice = iface.showMenu("What to show?", {
    "Show HP",
    "Show your hand",
    "Show your battlefield",
    "Show adversary battlefield",
    "Go back",
  });
  
  if(choice == 0) { // show HP
    auto msg = getPlayerName(&player1) + "'s HP: " + std::to_string(player1.getHP()) + ", "
             + getPlayerName(&player2) + "'s HP: " + std::to_string(player2.getHP()); 
    iface.tell(msg);
  }

  else if(choice == 1) { // show player hand
    auto msg = getPlayerName(player) + "'s hand";
    auto cards = player->getCardsInState<Card>(Card::State::HAND);

    if(cards.size() > 0)
      iface.showCards(msg, cards);
    else
      iface.tell("Hand is empty");
  }

  else if(choice == 2) { // show battlefield
    auto msg = getPlayerName(player) + "'s cards on the battlefield";
    auto cards = player->getCardsInState<Card>(Card::State::BATTLEFIELD);

    if(cards.size() > 0)
      iface.showCards(msg, cards);
    else
      iface.tell("Battlefield is empty");
  }

  else if(choice == 3) { // show adversary battlefield
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

    int choice = iface.showMenu(getPlayerName(player) + ", what to do?", {
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
      canPlaceLand = !placeLand(player);
      canAttack = !hasAttacked && hasAttackCreatures();
    }
    else if(choice == 2) { // place cards
      placeCreature(player);
      canAttack = !hasAttacked && hasAttackCreatures();
    }
    else if(choice == 3) { // attack
      hasAttacked = attackPhase(player);
      canAttack &= !hasAttacked;
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
