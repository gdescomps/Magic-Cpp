#pragma once

#include <string>
#include <array>

#include "Card.hpp"
#include "Mana.hpp"

class Land : public Card {

public:
  Land(Mana mana) : mana(mana) {} 

  Mana getMana() const { return mana; }
  
  std::string getName() const override { 
    static std::array<std::string, 5> const names = {
      "Plains", "Island", "Swamp", "Mountain", "Forest"
    };

    return names[(size_t)mana]; // using enum indexing order in the lookup of names
  }
  
  std::string getType() const override {
    static std::array<std::string, 5> const names = {
      "White", "Blue", "Black", "Red", "Forest"
    };
    return "Land - " + names[(size_t)mana] + " mana"; // using enum indexing order in the lookup of names
  }
  
  ManaCost getCost() const override { return ManaCost(); }
private:
  Mana mana;
};

