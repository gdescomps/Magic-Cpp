#include "Card.hpp"

#include <iostream>
#include <algorithm>

Card::Card()
  : state(State::LIBRARY)
{}

std::string Card::stateToString(State s) {
  constexpr std::array lookup = {
    "Library", "Hand", "Graveyard", "Battlefield"
  };
  return lookup[(size_t)s];
}

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
