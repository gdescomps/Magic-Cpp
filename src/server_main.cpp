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
    GameServer s;
    s.init();
 
    Interface iface(&s);

    Game game(iface);
    game.play();

    return 0;
}
