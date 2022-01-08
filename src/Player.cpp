#include "Player.hpp"

#include <algorithm>

Player::Player(Cards deck)
  : deck(move(deck)) 
{}

Card* Player::drawCard() {
  auto pred = [] (auto& c) { return c->getState() == Card::State::LIBRARY; };
  auto card = std::find_if(deck.begin(), deck.end(), pred);

  if(card == deck.end()) {
    // no card in library
    return nullptr;
  }
  else {
    (*card)->setState(Card::State::HAND);
    return (*card).get();
  }
}

std::vector<Card*>Player::getCards() {
  std::vector<Card*> res(deck.size());
  for(size_t i = 0; i < deck.size(); i++) {
    res[i] = deck[i].get();
  }
  return res;
}

std::vector<Card*>Player::getCardsInState(Card::State state) {
  auto cards = getCards();
  auto pred = [=] (Card* c) { return c->getState() == state; };
  cards.erase(std::remove_if(cards.begin(), cards.end(), pred), cards.end());
  return cards;
}

std::vector<Card*> Player::getPlaceableCards(ManaCost const& cost) {
  auto cards = getCards();
  
  auto isPlaceable = [&] (auto& c) {
    return c->getState() == Card::State::HAND 
        && cost.matches(c->getCost());
  };
  cards.erase(std::remove_if(cards.begin(), cards.end(), isPlaceable), cards.end());   

  return cards;
}

void Player::untapAll() {
  for(auto& card: deck) {
    if(card->getState() == Card::State::BATTLEFIELD) {
      card->untap();
    }
  }
}