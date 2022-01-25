#include "../lib/httplib.hpp"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <iostream>
#include <string>
#include "UserInterface.hpp"

using namespace std;
using namespace rapidjson;

int main(void) {

  httplib::Client cli("localhost", 8080);
  UserInterface ui;

  // if ( auto res = cli.Get("/attacker/4") ) {
  //   cout << res->status << endl;
  //   cout << res->get_header_value("Content-Type") << endl;
  //   cout << res->body << endl;
  // } else {
  //   cout << "error code: " << res.error() << std::endl;
  // }

  // if ( auto res = cli.Get("/blockers/1,2,3") ) {
  //   cout << res->status << endl;
  //   cout << res->get_header_value("Content-Type") << endl;
  //   cout << res->body << endl;
  // } else {
  //   cout << "error code: " << res.error() << std::endl;
  // }

  if ( auto res = cli.Get("/poll/0") ) {
    // cout << res->status << endl;
    // cout << res->get_header_value("Content-Type") << endl;
    // cout << res->body << endl;

    // 1. Parse a JSON string into DOM.
    const char* json = res->body.c_str();
    Document d;
    d.Parse(json);

    // 2. Modify it by DOM.
    Value& s = d["dataType"];

    string dataType = s.GetString();

    // cout << dataType << endl;

    if(dataType.compare("menu") == 0){
      // cout << "menu received"<< endl;
      string msg = d["msg"].GetString();

      auto entries = d["entries"].GetArray();
      
      std::vector<MenuEntry> menuEntries;

      for(SizeType i = 0; i < entries.Size(); i++) {
        // cout << entries[i].GetObject()["text"].GetString() << endl;

        string text = entries[i].GetObject()["text"].GetString();
        MenuEntry::State state = static_cast<MenuEntry::State>(entries[i].GetObject()["state"].GetInt());

        menuEntries.push_back(MenuEntry(text, state));

      }

      ui.showMenu(msg, menuEntries);
      
    }


  } else {
    cout << "error code: " << res.error() << std::endl;
  }


  return 0;
}
