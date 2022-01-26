#include <iostream>
#include <vector>

#include "Game.hpp"
#include "GameServer.hpp"
#include "Interface.hpp"
#include "creatures/AirBender.hpp"
#include "creatures/BrandonSoupSirup.hpp"

#include "Land.hpp"

int main()
{
    auto s = GameServer();
    s.init();

        
    Interface iface(&s);
    // i.tell("test message");

    // std::cout << MenuEntry("Test choice", MenuEntry::State::NORMAL).toString() << std::endl;

    // int choice = iface.showMenu("What to do?", {
    //   "Show...",
    //   MenuEntry("Place a Land", MenuEntry::State::DISABLED),
    //   "Place a Creature",
    //   MenuEntry("Attack", MenuEntry::State::NORMAL),
    //   "Finish turn",
    // });

    // std::cout << "choice" << choice << std::endl;

    Game game(iface);
    game.play();

    // std::vector<Card*> cards;
    // cards.push_back(new AirBender());
    // cards.push_back(new BrandonSoupSirup());

    // iface.showCards("card to show", cards);

    // AirBender c;

    // iface.showCard(&c);

    // Land l(Mana::GREEN);

    // iface.showCard(&l);


    return 0;

}
