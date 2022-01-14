#include <iostream>

#include "Game.hpp"
#include "Duel.hpp"
#include "creatures/SerraAngel.hpp"
#include <memory>


int main(int /* argc */, char const** /* argv */) {


  Creature* cardAttacker = new SerraAngel();

  std::vector<Creature*> cardBlockers;
  cardBlockers.push_back(new SerraAngel());
  cardBlockers.push_back(new SerraAngel());

  Player* playerAttacker;
  Player* playerBlocker;

  Duel d1 = Duel(cardAttacker, cardBlockers, playerAttacker, playerBlocker);

  std::cout << "attacker: "<< d1.getCardAttacker()->getName() << d1.getCardAttacker()->getPower() << "/" << d1.getCardAttacker()->getToughness() << '\n';

  for (auto blocker : d1.getCardBlockers()) {
      std::cout << "blocker: " << blocker->getName() << " " << blocker->getPower() << "/" << blocker->getToughness() << '\n';
  }

  std::cout << "Do the attack." << '\n';
  d1.getCardAttacker()->attack(&d1);

  std::cout << "attacker: "<< d1.getCardAttacker()->getName() << d1.getCardAttacker()->getPower() << "/" << d1.getCardAttacker()->getToughness() << '\n';

  for (auto blocker : d1.getCardBlockers()) {
      std::cout << "blocker: " << blocker->getName() << " " << blocker->getPower() << "/"  << blocker->getToughness() << '\n';
  }

  return 0;
}
