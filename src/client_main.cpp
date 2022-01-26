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

int main(void) {
  CardRegistry::registerAllCards();
  Client c;

  while (true)
  {
      c.poll();
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(2000ms);

  }
  

  return 0;
}
