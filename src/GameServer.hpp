#pragma once

#include "Card.hpp"
#include "Game.hpp"
#include "Duel.hpp"

class GameServer {

public:
    GameServer();
    virtual ~GameServer();

private:
    std::vector<Card*> deckP1, deckP2;

    void start();
    
    std::vector<Duel> duelHistory;

};

