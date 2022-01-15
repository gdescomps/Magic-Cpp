#pragma once

#include <string>

#include "Card.hpp"

class Creature : public Card {

public:
Creature();
virtual ~Creature();

virtual std::string getName() const override = 0;
virtual std::string getType() const override { return "Creature"; }
virtual int getPower() const = 0;
virtual int getToughness() const = 0;
 
private:

};

