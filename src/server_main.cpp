#include <iostream>

#include "GameServer.hpp"
// #include "Interface.hpp"


int main()
{
    auto s = GameServer();
        
    Interface iface(&s);
    // i.tell("test message");

    // std::cout << MenuEntry("Test choice", MenuEntry::State::NORMAL).toString() << std::endl;

    iface.showMenu("What to do?", {
      "Show...",
      MenuEntry("Place a Land", MenuEntry::State::DISABLED),
      "Place a Creature",
      MenuEntry("Attack", MenuEntry::State::NORMAL),
      "Finish turn",
    });

    s.start();

    return 0;

}
