#pragma once

#include "Card.hpp"
#include "Game.hpp"

class GameServer {

public:
    GameServer();
    virtual ~GameServer();

private:
    Cards deckP1, deckP2;

    void start();
    

};

