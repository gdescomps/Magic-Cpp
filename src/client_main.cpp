// #include "../lib/httplib.hpp"
// #include "rapidjson/document.h"
// #include "rapidjson/writer.h"
// #include "rapidjson/stringbuffer.h"

// #include <iostream>
// #include <string>
// #include "UserInterface.hpp"

#include "Client.hpp"

// using namespace std;
// using namespace rapidjson;

int main(void) {
  Client c;
  
  c.poll();
  return 0;
}
