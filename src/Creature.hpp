#pragma once

#include <string>
#include <algorithm>

#include "Card.hpp"
#include "Duel.hpp"

class Ability;

class Creature : public Card {

private:
    int power, basePower;
    int toughness, baseToughness;
    std::vector<Ability*> abilities;

public:
    Creature(int power, int toughness, std::vector<Ability*> abilities);
    Creature(int power, int toughness);

    virtual std::string getName() const override = 0;
    virtual std::string getType() const override { return "Creature"; }

    int getBasePower() const { return this->basePower; }
    int getPower() const { return this->power; }
    void setPower(int p) { this->power = p; }

    int getBaseToughness() const { return this->baseToughness; }
    int getToughness() const { return this->toughness; }
    void setToughness(int t) { this->toughness = t; }

    DuelValidation validateAttack(Duel const& duel);
    DuelValidation validateBlock(Duel const& duel);

    std::vector<Ability*> getAbilities() const { return this->abilities; }
    bool hasAbility(Ability* ability) const;
    template<class A>
    bool hasAbility() const;
    
    bool isFirstTurn() const { return false; /* TODO */ }
};

template<class A>
inline bool Creature::hasAbility() const {
  auto it = std::ranges::find_if(abilities, [] (Ability* a) {
      return dynamic_cast<A>(a) != nullptr;
  });
  return it != abilities.end();
}