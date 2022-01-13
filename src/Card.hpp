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
  
  virtual std::string getName() const = 0;
  virtual std::string getType() const = 0;
  virtual ManaCost getCost() const = 0;
  virtual Mana getMana() const = 0;
private:
  State state;
  bool tapped;
};
