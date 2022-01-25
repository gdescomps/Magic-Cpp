#pragma once

#include "Mana.hpp"

class Ability;

/** A card. (abstract class) */
class Card {

public:
  enum class State {
    LIBRARY, HAND, GRAVEYARD, BATTLEFIELD
  };

  Card();
  virtual ~Card();
  Card(Card const&) = delete;
  Card& operator=(Card const&) = delete;
  
  State getState() const;
  void setState(State state);
  void tap();
  void untap();
  bool isTapped() const;
  bool hasAbility(Ability const* ability) const;
  
  virtual std::string getName() const = 0;
  virtual std::string getType() const = 0;
  virtual std::string getDesc() const { return "No description available."; }
  virtual ManaCost getCost() const = 0;
  virtual Mana getMana() const = 0;
  virtual std::vector<Ability const*> getAbilities() const { return {}; }
  virtual int getCardId() const = 0;
private:
  State state;
  bool tapped;
};
