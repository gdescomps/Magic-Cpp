#include <iostream>

#include "GameServer.hpp"
// #include "Interface.hpp"


int main()
{
    auto s = GameServer();
        
    Interface i(&s);
    // i.tell("test message");

    // std::cout << MenuEntry("Test choice", MenuEntry::State::NORMAL).toString() << std::endl;

    i.showMenu("What to show?", {
        "Show HP",
        "Show your hand",
        "Show your battlefield",
        "Show adversary battlefield",
        "Go back",
    });

    s.start();

    return 0;

}
