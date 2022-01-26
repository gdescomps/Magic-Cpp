#include <iostream>

#include "GameServer.hpp"
#include "Interface.hpp"
#include "creatures/AirBender.hpp"
#include "Land.hpp"

int main()
{
    auto s = GameServer();
        
    Interface iface(&s);
    // i.tell("test message");

    // std::cout << MenuEntry("Test choice", MenuEntry::State::NORMAL).toString() << std::endl;

    // iface.showMenu("What to do?", {
    //   "Show...",
    //   MenuEntry("Place a Land", MenuEntry::State::DISABLED),
    //   "Place a Creature",
    //   MenuEntry("Attack", MenuEntry::State::NORMAL),
    //   "Finish turn",
    // });

    AirBender c;

    iface.showCard(&c);

    Land l(Mana::BLACK);

    iface.showCard(&l);

    s.start();

    return 0;

}
