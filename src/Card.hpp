#pragma once

#include "Mana.hpp"

class Card {

public:
  enum class State {
    LIBRARY, HAND, GRAVEYARD, BATTLEFIELD
  };

  Card();
  virtual ~Card();

  State getState() const;
  void setState(State state);
  void tap();
  void untap();
  bool isTapped() const;

  virtual ManaCost getCost() const = 0;

private:
  State state;
  bool tapped;
};

