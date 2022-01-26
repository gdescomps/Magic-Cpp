#include "Player.hpp"

#include <algorithm>

Player::Player(Cards deck)
  : deck(move(deck)), hp(20)
{}


// TODO : draw random card
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

void Player::untapAll() {
  for(auto& card: deck) {
    if(card->getState() == Card::State::BATTLEFIELD) {
      card->untap();
    }
  }
}