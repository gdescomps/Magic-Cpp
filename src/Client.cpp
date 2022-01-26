#include "Client.hpp"

#include "../lib/httplib.hpp"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <iostream>
#include <string>
#include "UserInterface.hpp"
#include "CardRegistry.hpp"

using namespace std;
using namespace rapidjson;



Client::Client(){

    this->cli = new httplib::Client("localhost", 8080);
    this->ui = new UserInterface();

    this->playerI = 0; // TODO : Demander le numéro au serveur

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

}

Client::~Client(){
    delete this->cli;
    delete this->ui;
}

void Client::poll(){

  std::string request = "/poll/" + std::to_string(playerI);

  if ( auto res = cli->Get(request.c_str()) ) {
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

      this->ui->showMenu(msg, menuEntries);
      
    }

    else if(dataType.compare("card") == 0){

      std::string name = d["id"].GetString();


      // auto card = CardRegistry::getInst().create<Card>(cardId);
      auto card = CardRegistry::getInst().create<Card>(name);
      
      // card.get()->fromJson("{\"dataType\":\"card\",\"name\":\"Forest\",\"isTapped\":false,\"state\":0,\"mana\":4}");
      card.get()->fromJson(res->body);

      // ui->tell("testdsiuh");

      ui->showCard(card.get());

    }


  } else {
    cout << "error code: " << res.error() << std::endl;
  }
}
