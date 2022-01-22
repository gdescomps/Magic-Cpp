#pragma once

#include <vector>
#include <memory>

#include "Card.hpp"
#include "Land.hpp"
#include "Creature.hpp"
#include "Mana.hpp"

using Cards = std::vector<std::unique_ptr<Card>>; 

class Player {
public:
  Player(Cards deck);
    
  Player(Player const&) = delete;
  Player& operator=(Player const&) = delete;
  
  Card* drawCard();
  
  void untapAll();

  int getHP() const { return hp; }
  void setHP(int hp) { this->hp = hp; }
  
  template<class C>
  std::vector<C*> getCardsInState(Card::State state);
  
  template<class C>
  std::vector<C*> getPlaceableCards(ManaCost const& cost);
  
  template<class C>
  std::vector<C*> getCards();
  

private:
  Cards deck;
  int hp;
};

template<class C>
inline std::vector<C*> Player::getCards() {
  std::vector<C*> res;

  for(size_t i = 0; i < deck.size(); i++) {
    C* casted = dynamic_cast<C*>(deck[i].get());
    if(casted != nullptr) res.push_back(casted);
  }

  return res;
}

template<>
inline std::vector<Card*> Player::getCards() {
  std::vector<Card*> res(deck.size());

  for(size_t i = 0; i < deck.size(); i++) {
    res[i] = deck[i].get();
  }

  return res;
}

template<class C>
inline std::vector<C*> Player::getCardsInState(Card::State state) {
  auto cards = getCards<C>();

  std::erase_if(cards, [=] (Card* c) {
    return c->getState() != state;
  });

  return cards;
}

template<class C>
std::vector<C*> Player::getPlaceableCards(ManaCost const& cost) {
  auto cards = getCards<C>();
  
  std::erase_if(cards, [&] (Card* c) {
    return c->getState() != Card::State::HAND || !cost.enough(c->getCost());
  });

  return cards;
}

