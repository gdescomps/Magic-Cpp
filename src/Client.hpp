#pragma once

#include "../lib/httplib.hpp"
// #include "UserInterface.hpp"

class UserInterface;

class Client {

public:
    Client(int player);
    ~Client();

    void poll();

    int playerI;

private:
    
    httplib::Client* cli;
    UserInterface* ui;

    
};
