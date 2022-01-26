#pragma once

#include "Card.hpp"
// #include "Game.hpp"
#include "Duel.hpp"
#include "../lib/httplib.hpp"

class GameServer {

public:
    GameServer();
    virtual ~GameServer();

    void send(std::string s);

    void init();
    void listen();

    int getChoice();
    std::vector<int> getMultiChoices();

private:
    std::vector<Card*> deckP1, deckP2;
    
    std::vector<Duel> duelHistory;
    std::vector<std::string> sendBuffer;

    std::vector<int> choice;
    std::vector<std::vector<int>> multiChoices;

    int currentPlayer;

    httplib::Server svr;
};

