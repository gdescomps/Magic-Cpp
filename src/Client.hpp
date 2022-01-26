#pragma once

#include "../lib/httplib.hpp"

class UserInterface;

/**
 * Receive data from the server and react accordingly by sending it to the user interface
 */
class Client {

public:

    /**
     * @brief Construct a new Client object
     * @param player playerID (0 or 1)
     */
    Client(int player);

    ~Client();

    /**
     * @brief Send a request to the server to get what he has to send to this client
     */
    void poll();

    /** playerID (0 or 1) */
    int playerI;

private:
    
    /** The httplib client instance */
    httplib::Client* cli;

    /** The UserInterface instance */
    UserInterface* ui;

    
};
