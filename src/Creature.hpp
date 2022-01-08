#pragma once

#include <string>

#include "Card.hpp"

class Creature : public Card {

public:
Creature();
virtual ~Creature();

virtual std::string getName() const = 0;
virtual std::string getType() const { return "Creature"; }
virtual int getPower() const = 0;
virtual int getToughness() const = 0;

private:

};

