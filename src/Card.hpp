#pragma once

#include "Mana.hpp"

/** A card. (abstract class) */
class Card {

public:
  enum class State {
    LIBRARY, HAND, GRAVEYARD, BATTLEFIELD
  };
  
  static std::string stateToString(State s);

  Card();
  virtual ~Card() = default;
  Card(Card const&) = delete;
  Card& operator=(Card const&) = delete;
  
  State getState() const;
  void setState(State state);
  void tap();
  void untap();
  bool isTapped() const;
  
  virtual std::string getName() const = 0;
  virtual std::string getType() const = 0;
  virtual std::string getDesc() const { return "No description available."; }
  virtual ManaCost getCost() const = 0;
  virtual Mana getMana() const = 0;
private:
  State state;
  bool tapped;
};
