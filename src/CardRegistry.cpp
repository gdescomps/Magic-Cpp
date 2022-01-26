#include "CardRegistry.hpp"

#include "Land.hpp"
#include "creatures/AirBender.hpp"
#include "creatures/BrandonSoupSirup.hpp"
#include "creatures/CrownTheVirulent.hpp"
#include "creatures/JackSeparou.hpp"
#include "creatures/LeojTheMerciful.hpp"
#include "creatures/PolyNinja.hpp"
#include "creatures/RatMan.hpp"
#include "creatures/SerraAngel.hpp"
#include "creatures/Theresa.hpp"

void CardRegistry::registerAllCards() {
  auto& reg = CardRegistry::getInst();
  reg.registerCard<Land>();
  reg.registerCard<AirBender>();
  reg.registerCard<BrandonSoupSirup>();
  reg.registerCard<CrownTheVirulent>();
  reg.registerCard<JackSeparou>();
  reg.registerCard<LeojTheMerciful>();
  reg.registerCard<PolyNinja>();
  reg.registerCard<RatMan>();
  reg.registerCard<SerraAngel>();
  reg.registerCard<Theresa>();
}
