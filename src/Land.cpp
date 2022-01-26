#include "Land.hpp"

#include <string>
#include <array>

#include "CardRegistry.hpp"

Land::Land(Mana mana) : mana(mana) {}
Land::Land() : Land(Mana::BLACK) {}

Mana Land::getMana() const { return mana; }

std::string Land::getName() const {
  static std::array<std::string, 5> const names = {
    "Plains", "Island", "Swamp", "Mountain", "Forest"
  };

  return names[(size_t)mana]; // using enum indexing order in the lookup of names
}

std::string Land::getType() const {
  static std::array<std::string, 5> const names = {
    "White", "Blue", "Black", "Red", "Forest"
  };
  return "Land - " + names[(size_t)mana] + " mana"; // using enum indexing order in the lookup of names
}

ManaCost Land::getCost() const { return ManaCost(); }
