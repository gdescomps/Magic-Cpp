#pragma once

#include "../lib/httplib.hpp"
// #include "UserInterface.hpp"

class UserInterface;

class Client {

public:
    Client();
    ~Client();

    void poll();

private:
    
    httplib::Client* cli;
    UserInterface* ui;
};
