#pragma once

#include "../lib/httplib.hpp"
// #include "UserInterface.hpp"

class UserInterface;

class Client {

public:
    Client();
    ~Client();

    void poll();

    int playerI;

private:
    
    httplib::Client* cli;
    UserInterface* ui;

    
};
