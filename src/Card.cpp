#include "Card.hpp"

#include <iostream>

Card::Card()
  : state(State::LIBRARY)
{

}

Card::~Card() {}

void Card::setState(State state) {
  this->state = state;
}

Card::State Card::getState() const {
  return state;
}

void Card::tap() {
  tapped = true;
}

void Card::untap() {
  tapped = false;
}

bool Card::isTapped() const {
  return tapped;
}