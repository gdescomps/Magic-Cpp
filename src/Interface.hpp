#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "Card.hpp"
#include "Creature.hpp"
#include "Land.hpp"

#include "GameServer.hpp"

// class GameServer;

/** A menu entry. 
    Can be created from a string with default state of 'NORMAL', or also provide a state. */
class MenuEntry {

public:
  enum class State { NORMAL=0, SELECTED=1, DISABLED=2 };  

  MenuEntry(std::string text, State state)
   : text(text), state(state)
  {}
  
  MenuEntry(char const* text)
   : text(text), state(State::NORMAL)
  {}

  std::string toString(){
    std::stringstream sstm;
    sstm << static_cast<int>(state) << "§" << text;
    
    return sstm.str();
  }
  
  std::string text;
  State state; 
};

/** The game terminal-based user interface.
    It uses the library ncurse to manage drawing on the terminal.
    The ui is composed of a header bar (title), a main screen and a prompt. */
class Interface {
public:
  Interface(GameServer* s) : server(s) {};
  
  /** Show a card details centered on the screen.
      Note: call hideAll() to hide */  
  template<class C>
  void showCard(int player, C const* c);
  
  /** Show cards details centered on the screen.
      Note: call hideAll() to hide */
  template<class C>
  void showCards(int player, std::string const& msg, std::vector<C*> const& cards);

  /** Show a selector menu to select a single card in a list.
      @param msg a message to prompt the user.
      @param cards a list of cards to choose from.
      @return the selected card or nullptr if selection was cancelled. */  
  template<class C>
  C* selectCard(int player, std::string const& msg, std::vector<C*> const& cards);

  /** Show a selector menu to select a single card in a list.
      @param msg a message to prompt the user.
      @param cards a list of cards to choose from.
      @return a possibly empty list of selected cards. */  
  template<class C>
  std::vector<C*> selectCards(int player, std::string const& msg, std::vector<C*> const& cards);
  
  /** Hide content drawn on the main screen (not the header nor the prompt). */
  void hideAll(int player);
  
  /** Tell a message to the user. */
  void tell(int player, std::string const& msg);

  /** Ask a yes / no question to the user. 
      @param msg the question to ask.
      @return true if answer was 'yes', false if answer was 'no'. */
  bool promptYesNo(int player, std::string const& msg);

  /** Show a menu. 
      @param msg the displayed menu name.
      @param choices the different menu entries.
      @return the selected entry index in choices, or -1 if cancelled. */
  int showMenu(int player, std::string const& msg, std::vector<MenuEntry> choices);
  
  void setPlayer(int player);

private:
  GameServer* server;
};


template <class C>
inline void Interface::showCard(int player, C const *card)
{
  this->server->send(player, card->toJson());
}

template<class C>
inline void Interface::showCards(int player, std::string const& msg, std::vector<C*> const& cards)
{
  using namespace rapidjson;
  StringBuffer s;
  Writer<StringBuffer> writer(s);

  writer.StartObject();
  writer.Key("dataType");
  writer.String("showCards");
  writer.Key("msg");
  writer.String(msg.c_str());
  writer.Key("cards");
  writer.StartArray();


  for (auto c : cards)
  {
    std::string cardJson = c->toJson();
    writer.RawValue(cardJson.c_str(), cardJson.size(), rapidjson::kObjectType);
    
  }

  writer.EndArray();
  writer.EndObject();

  // std::cout << s.GetString() << std::endl;

  this->server->send(player, s.GetString());

}

template<class C>
inline C* Interface::selectCard(int player, std::string const& msg, std::vector<C*> const& cards){
  using namespace rapidjson;
  StringBuffer s;
  Writer<StringBuffer> writer(s);

  writer.StartObject();
  writer.Key("dataType");
  writer.String("selectCard");
  writer.Key("msg");
  writer.String(msg.c_str());
  writer.Key("cards");
  writer.StartArray();


  for (auto c : cards)
  {
    std::string cardJson = c->toJson();
    writer.RawValue(cardJson.c_str(), cardJson.size(), rapidjson::kObjectType);
    
  }

  writer.EndArray();
  writer.EndObject();

  // std::cout << s.GetString() << std::endl;

  this->server->send(player, s.GetString());
  int choice = this->server->getChoice(player);
  return choice == -1 ? nullptr : cards[choice];
}

template<class C>
inline std::vector<C*> Interface::selectCards(int player, std::string const& msg, std::vector<C*> const& cards){
  using namespace rapidjson;
  StringBuffer s;
  Writer<StringBuffer> writer(s);

  writer.StartObject();
  writer.Key("dataType");
  writer.String("selectCards");
  writer.Key("msg");
  writer.String(msg.c_str());
  writer.Key("cards");
  writer.StartArray();


  for (auto c : cards)
  {
    std::string cardJson = c->toJson();
    writer.RawValue(cardJson.c_str(), cardJson.size(), rapidjson::kObjectType);
    
  }

  writer.EndArray();
  writer.EndObject();

  // std::cout << s.GetString() << std::endl;

  this->server->send(player, s.GetString());
  auto choices = this->server->getMultiChoices(player);
  std::vector<C*> res(choices.size());
  for(size_t i = 0; i < choices.size(); i++) {
    res[i] = cards[choices[i]];
  }
  return res;
}