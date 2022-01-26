#pragma once

#include "Card.hpp"
// #include "Game.hpp"
#include "Duel.hpp"
#include "../lib/httplib.hpp"

/** The server of the game it sends informations to clients and receive requests with an http API **/
class GameServer
{

public:
    GameServer();
    virtual ~GameServer();

    /**
     * Send a string to a player
     * @param player playerID (0 or 1)
     * @param s the string to send
     */
    void send(int player, std::string s);

    /**
     * Define the request that can be made to the API and how they react
     */
    void init();
    
    /**
     * Put the server in a state to wait for a request
     * (it is blocking for the server)
     */
    void listen();

    /**
     * Receive a choice from the player
     * 
     * @param player playerID (0 or 1)
     * @return int the choice of the player
     */
    int getChoice(int player);

    /**
     * Receive multiple choices from the player
     * 
     * @param player playerID (0 or 1)
     * @return std::vector<int> choice(s) of the player
     */
    std::vector<int> getMultiChoices(int player);

private:

    /** Stores data to send to players, the elements id of the vector correspond to the player id */
    std::vector<std::string> sendBuffer;

    /** Store a choice made by players, the elements id of the vector correspond to the player id */
    std::vector<int> choice;

    /** Store multiple choices made by players, the elements id of the vector correspond to the player id */
    std::vector<std::vector<int>> multiChoices;

    /** The httplib server instance */
    httplib::Server svr;
};
