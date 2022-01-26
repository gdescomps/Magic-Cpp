#pragma once

#include "Card.hpp"
// #include "Game.hpp"
#include "Duel.hpp"
#include "../lib/httplib.hpp"

class GameServer {

public:
    GameServer();
    virtual ~GameServer();

    void send(int player, std::string s);

    void init();
    void listen();

    int getChoice(int player);
    std::vector<int> getMultiChoices(int player);

private:
    std::vector<Card*> deckP1, deckP2;
    
    std::vector<Duel> duelHistory;
    std::vector<std::string> sendBuffer;

    std::vector<int> choice;
    std::vector<std::vector<int>> multiChoices;


    httplib::Server svr;
};

