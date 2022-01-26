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
#include "creatures/Tau.hpp"
#include "creatures/Thanos.hpp"

#include "abilities/Haste.hpp"
#include "abilities/Vigilance.hpp"

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
  res.push_back(std::make_unique<Tau>());
  res.push_back(std::make_unique<Thanos>());
  res.push_back(std::make_unique<AirBender>());
  res.push_back(std::make_unique<BrandonSoupSirup>());
  res.push_back(std::make_unique<CrownTheVirulent>());
  res.push_back(std::make_unique<JackSeparou>());
  res.push_back(std::make_unique<LeojTheMerciful>());
  res.push_back(std::make_unique<PolyNinja>());
  res.push_back(std::make_unique<RatMan>());
  res.push_back(std::make_unique<SerraAngel>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Tau>());
  res.push_back(std::make_unique<Thanos>());
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
  for(size_t i = 0; i < 11; i++) {
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
  
  Player* winner = player1.getHP() == 0 ? &player2 : &player1;
  iface.tell("Game has ended, " + getPlayerName(winner) + " wins.");
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
  ManaCost avail;

  // get available mana for the player
  auto lands = player->getCardsInState<Land>(Card::State::BATTLEFIELD);
  std::erase_if(lands, [] (Card* c) { return c->isTapped(); });
  for(auto land : lands) {
    avail.add(land->getMana());
  }

  // find placeable creatures
  auto placeables = player->getPlaceableCards<Creature>(avail);
  if(placeables.size() == 0) {
    iface.tell("No placeable creature.");
    return false;
  }

  // ask creature to place
  auto selected = iface.selectCard("Which creature do you wish to place?", placeables);
  if(selected == nullptr) return false;
  
  // tap specific manas
  ManaCost remain = selected->getCost();
  for(auto it = lands.begin(); it != lands.end(); ) {
    Land* land = *it;
    if(remain.get(land->getMana()) > 0) {
      land->tap();
      remain.set(land->getMana(), remain.get(land->getMana()) - 1);
      it = lands.erase(it);
    }
    else ++it;
  }
  
  // tab any mana
  while(remain.getAny() > 0) {
    auto selected = iface.selectCards("Select lands to tap (" + std::to_string(remain.getAny()) + " missing).", lands);

    if((int)selected.size() > remain.getAny()) {
      iface.tell("Too many lands tapped.");
      continue;
    }

    for(auto land : selected) {
      land->tap();
      remain.setAny(remain.getAny() - 1);
      lands.erase(std::ranges::find(lands, land));
    }
  }

  // place the creature
  if(!selected->hasAbility<Haste>())
    selected->tap();
  selected->setState(Card::State::BATTLEFIELD);
  return true;
}

bool Game::attackPhase(Player* player) {
  Player* adversary = switchPlayer(player);
  auto p1Name = getPlayerName(player);
  auto p2Name = getPlayerName(adversary);
  std::vector<Duel> duels;

  // find creatures usable in attack
  auto availAttackers = player->getCardsInState<Creature>(Card::State::BATTLEFIELD);
  std::erase_if(availAttackers, [] (Card* c) { return c->isTapped(); });

  if(availAttackers.size() == 0) {
    iface.tell("No creature to use in attack");
    return false;
  }

  // ask creatures to use
  std::vector<Creature*> attackers = iface.selectCards<Creature>("Select creatures to use in attack", availAttackers);
  if(attackers.size() == 0) return false;

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
    
    // verify duel
    Duel duel(attacker, blockers, player, adversary);
    auto validation = duel.validateDuel();
    if(!validation) {
      iface.tell("The duel was refused because " + validation.getMsg());
      continue;
    }
    
    // create the duel and remove attacker / blockers
    duels.push_back(duel);
    if(blockers.size() != 0) {
      attackers.erase(std::ranges::find(attackers, attacker));
      for(auto blocker : blockers) {
        availBlockers.erase(std::ranges::find(availBlockers, blocker));
      }
    }

    // ask player to order attacks for duels with multiple blockers
    for(auto& duel : duels) if(duel.blockers.size() >= 2) {
      for(size_t i = 0; i < duel.blockers.size() - 1; i++) {
        Card* card;
        std::vector<Creature*> remainingBlockers(duel.blockers.begin() + i, duel.blockers.end());
        do card = iface.selectCard("Choose the " + getOrdinal(i+1) + " card to attack with your " + duel.attacker->getName(), remainingBlockers);
        while(card == nullptr);
        std::iter_swap(duel.blockers.begin() + i, std::ranges::find(duel.blockers, card));
      }
    }
  }
  
  // add remaining attackers
  for(Creature* attacker : attackers) {
    duels.emplace_back(attacker, std::vector<Creature*>{}, player, adversary);
  }

  // Perform the DUELS
  for(auto& duel : duels) {
    duel.performDuel();
    
    if(duel.attacker->hasAbility<Vigilance>())
      duel.attacker->tap();
    
    // Move dead blockers to Graveyard
    std::vector<Creature*> deadBlockers = duel.blockers;
    std::erase_if(deadBlockers, [&] (Creature* blocker) {
      return blocker->isAlive();
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
    
    // reset cards stats
    duel.attacker->setPower(duel.attacker->getBasePower());
    duel.attacker->setToughness(duel.attacker->getBaseToughness());
    for(Creature* blocker : duel.blockers) {
      blocker->setPower(blocker->getBasePower());
      blocker->setToughness(blocker->getBaseToughness());
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
    "Show your graveyard",
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

  else if(choice == 4) { // show graveyard
    auto msg = getPlayerName(player) + "'s cards in graveyard";
    auto cards = player->getCardsInState<Card>(Card::State::GRAVEYARD);

    if(cards.size() > 0)
      iface.showCards(msg, cards);
    else
      iface.tell("Graveyard is empty");
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
    
    // stop if a player is dead
    cont = player1.getHP() != 0 && player2.getHP() != 0;
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
