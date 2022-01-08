#pragma once

#include <vector>
#include <memory>

#include "Card.hpp"
#include "Land.hpp"
#include "Creature.hpp"
#include "Mana.hpp"

class Player {

using Cards = std::vector<std::unique_ptr<Card>>; 

public:
Player(Cards deck);

Card* drawCard();

std::vector<Card*> getCards();
std::vector<Card*> getCardsInState(Card::State state);
std::vector<Card*> getPlaceableCards(ManaCost const& cost);
std::vector<Land*> getLandCards();
std::vector<Creature*> getCreatureCards();

void untapAll();

private:
  Cards deck;
};

