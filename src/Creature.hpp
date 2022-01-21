#pragma once

#include <string>

#include "Card.hpp"

#include "Duel.hpp"

class Creature : public Card {

private:
    int power;
    int toughness;
    int initialToughness;

public:
    Creature(int power, int toughness) : power(power), toughness(toughness), initialToughness(toughness){}
    virtual ~Creature();

    virtual std::string getName() const override = 0;
    virtual std::string getType() const override { return "Creature"; }

    int getPower() const { return this->power; }

    int getToughness() const { return this->toughness; }
    void setToughness(int t) { this->toughness = t; }

    virtual void attack(Creature* adversary);

};
