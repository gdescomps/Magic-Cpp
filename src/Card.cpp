#include "Card.hpp"

#include <iostream>

Card::Card() {

}

Card::~Card() {}

void Card::print() const {
  std::cout << "Card !" << std::endl;
}