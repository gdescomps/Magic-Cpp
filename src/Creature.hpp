#pragma once

#include <string>

#include "Card.hpp"

#include "Duel.hpp"

class Creature : public Card {

private:
    int power, basePower;
    int toughness, baseToughness;

public:
    Creature(int power, int toughness) 
        : power(power), basePower(power), toughness(toughness), baseToughness(toughness) {}

    virtual std::string getName() const override = 0;
    virtual std::string getType() const override { return "Creature"; }

    int getBasePower() const { return this->basePower; }
    int getPower() const { return this->power; }
    void setPower(int p) { this->power = p; }

    int getBaseToughness() const { return this->baseToughness; }
    int getToughness() const { return this->toughness; }
    void setToughness(int t) { this->toughness = t; }

    virtual void attack(Creature* adversary);

};
