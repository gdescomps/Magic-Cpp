#pragma once

#include "Card.hpp"
// #include "Game.hpp"
#include "Duel.hpp"

class GameServer {

public:
    GameServer();
    virtual ~GameServer();

    void send(std::string s);

    void start();

private:
    std::vector<Card*> deckP1, deckP2;
    
    std::vector<Duel> duelHistory;
    std::vector<std::string> sendBuffer;

    int currentPlayer;
};

