#pragma once

#include <string>
#include <array>

#include "Card.hpp"
#include "Mana.hpp"

class Land : public Card {

public:
  Land(Mana mana);
  Land();

  Mana getMana() const override;  
  std::string getName() const override;  
  std::string getType() const override;  
  ManaCost getCost() const override;
  int getCardId() const override;

private:
  Mana mana;
};

