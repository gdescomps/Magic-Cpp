#include "Client.hpp"

#include "../lib/httplib.hpp"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <iostream>
#include <string>
#include "UserInterface.hpp"
#include "CardRegistry.hpp"
#include "creatures/AirBender.hpp"

using namespace std;
using namespace rapidjson;



Client::Client(int player) : playerI(player) {
    this->cli = new httplib::Client("localhost", 8080);
    this->ui = new UserInterface();
}

Client::~Client(){
    delete this->cli;
    delete this->ui;
}

void Client::poll(){

  std::string request = "/poll/" + std::to_string(playerI);

  if ( auto res = cli->Get(request.c_str()) ) {
    if(res.value().body == "void") return;
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

      int choice = this->ui->showMenu(msg, menuEntries);
      
      std::string request = "/choice/" + std::to_string(playerI) + "/" + std::to_string(choice);
      cli->Get(request.c_str());

    }

    else if(dataType.compare("promptYesNo") == 0){

      string msg = d["msg"].GetString();

      int choice = this->ui->promptYesNo(msg) ? 1 : 0;
      
      std::string request = "/choice/" + std::to_string(playerI) + "/" + std::to_string(choice);
      cli->Get(request.c_str());
    }

    else if(dataType.compare("tell") == 0){
      string msg = d["msg"].GetString();
      this->ui->tell(msg);
    }

    else if(dataType.compare("hideAll") == 0){
      ui->hideAll();
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

    else if(dataType.compare("showCards") == 0){

      string msg = d["msg"].GetString();

      auto entries = d["cards"].GetArray();
      
      std::vector<std::unique_ptr<Card>> ucards;
      std::vector<Card*> cards;

      for(SizeType i = 0; i < entries.Size() ; i++) {

        rapidjson::StringBuffer buffer;

        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        entries[i].Accept(writer);

        rapidjson::Document result;
        rapidjson::StringStream s(buffer.GetString());
        result.ParseStream(s);

        std::string cardJson = buffer.GetString();

        std::string name = entries[i].GetObject()["id"].GetString();

        auto card = CardRegistry::getInst().create<Card>(name);
        cards.push_back(card.get());
        ucards.push_back(move(card));
      
      }

      ui->showCards(msg, cards);
    }

    else if(dataType.compare("selectCard") == 0){

      string msg = d["msg"].GetString();

      auto entries = d["cards"].GetArray();
      
      std::vector<std::unique_ptr<Card>> ucards;
      std::vector<Card*> cards;

      for(SizeType i = 0; i < entries.Size() ; i++) {

        rapidjson::StringBuffer buffer;

        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        entries[i].Accept(writer);

        rapidjson::Document result;
        rapidjson::StringStream s(buffer.GetString());
        result.ParseStream(s);

        std::string cardJson = buffer.GetString();

        std::string name = entries[i].GetObject()["id"].GetString();

        auto card = CardRegistry::getInst().create<Card>(name);
        cards.push_back(card.get());
        ucards.push_back(move(card));
      
      }

      Card* card = ui->selectCard(msg, cards);
      auto it = std::ranges::find(cards, card);
      int index = it == cards.end() ? -1 : it - cards.begin();
      std::string request = "/choice/" + std::to_string(playerI) + "/" + std::to_string(index);
      cli->Get(request.c_str());
    }

    else if(dataType.compare("selectCards") == 0){

      string msg = d["msg"].GetString();

      auto entries = d["cards"].GetArray();
      
      std::vector<std::unique_ptr<Card>> ucards;
      std::vector<Card*> cards;

      for(SizeType i = 0; i < entries.Size() ; i++) {

        rapidjson::StringBuffer buffer;

        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        entries[i].Accept(writer);

        rapidjson::Document result;
        rapidjson::StringStream s(buffer.GetString());
        result.ParseStream(s);

        std::string cardJson = buffer.GetString();

        std::string name = entries[i].GetObject()["id"].GetString();

        auto card = CardRegistry::getInst().create<Card>(name);
        cards.push_back(card.get());
        ucards.push_back(move(card));
      
      }

      auto selected = ui->selectCards(msg, cards);
      string resp;
      if(selected.size() != 0) {
        resp = std::to_string(std::ranges::find(cards, selected[0]) - cards.begin());
        for(size_t i = 1; i < selected.size(); i++) {
          resp += ",";
          resp += std::to_string(std::ranges::find(cards, selected[i]) - cards.begin());
        } 
      }
      std::string request = "/multichoices/" + std::to_string(playerI) + "/" + resp;
      cli->Get(request.c_str());
    }


  } else {

    std::stringstream sstm;
    sstm << " error code: " << res.error();

    ui->tell(sstm.str());
  }
}
