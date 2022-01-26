// #include "../lib/httplib.hpp"
// #include "rapidjson/document.h"
// #include "rapidjson/writer.h"
// #include "rapidjson/stringbuffer.h"

// #include <iostream>
// #include <string>
// #include "UserInterface.hpp"

#include "Client.hpp"
#include "CardRegistry.hpp"
// using namespace std;
// using namespace rapidjson;

#include <chrono>
#include <thread>

int main(int argc, char const** argv) {
  
  if(argc != 2) {
    std::cout << "Usage: " << argv[0] << " <PlayerNum (0 or 1)>" << std::endl;
    return 1;
  }
  
  CardRegistry::registerAllCards();
  int player = std::stoi(argv[1]);
  Client c(player);

  while (true)
  {
      c.poll();
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(200ms);

  }
  

  return 0;
}
