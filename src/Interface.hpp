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
  ~Interface(){};
  
  /** Show a card details centered on the screen.
      Note: call hideAll() to hide */  
  template<class C>
  void showCard(C const* c);
  
//   /** Show cards details centered on the screen.
//       Note: call hideAll() to hide */
//   template<class C>
//   void showCards(std::string const& msg, std::vector<C*> const& cards);

//   /** Show a selector menu to select a single card in a list.
//       @param msg a message to prompt the user.
//       @param cards a list of cards to choose from.
//       @return the selected card or nullptr if selection was cancelled. */  
//   template<class C>
//   C* selectCard(std::string const& msg, std::vector<C*> const& cards);

//   /** Show a selector menu to select a single card in a list.
//       @param msg a message to prompt the user.
//       @param cards a list of cards to choose from.
//       @return a possibly empty list of selected cards. */  
//   template<class C>
//   std::vector<C*> selectCards(std::string const& msg, std::vector<C*> const& cards);
  
//   /** Hide content drawn on the main screen (not the header nor the prompt). */
//   void hideAll();
  
//   /** Tell a message to the user. */
  void tell(std::string const& msg);

//   /** Ask a yes / no question to the user. 
//       @param msg the question to ask.
//       @return true if answer was 'yes', false if answer was 'no'. */
//   bool promptYesNo(std::string const& msg);

  /** Show a menu. 
      @param msg the displayed menu name.
      @param choices the different menu entries.
      @return the selected entry index in choices, or -1 if cancelled. */
  int showMenu(std::string const& msg, std::vector<MenuEntry> choices);

private:
  GameServer* server;
};


template <class C>
inline void Interface::showCard(C const *card)
{
  // using namespace rapidjson;
  
  // // int cardI = std::find(server->deckP1.begin(), server->deckP1.end(), card); 

  // int classId = 1; // a modifier

  // StringBuffer s;
  // Writer<StringBuffer> writer(s);

  // writer.StartObject();
  // writer.Key("dataType");
  // writer.String("card");
  // writer.Key("type");
  // writer.Int(classId);
  // // writer.Key("data");

  // // writer.StartObject();
  // // writer.Key("name");
  // // writer.String(card->getName().c_str());
  // // writer.EndObject();

  // writer.EndObject();

  // std::cout << s.GetString() << std::endl;

  // Document cardDoc;
  // Pointer("/name").Set(cardDoc, card->getName());


  // Document d;
  // d.Parse(s.GetString());

  // Pointer("/data/name").Create(d);

  // // 2. Modify it by DOM.
  // Value& val = d["type"];
  // val.SetInt(val.GetInt() + 1);

  // // 3. Stringify the DOM
  // StringBuffer buffer;
  // Writer<StringBuffer> writer2(buffer);
  // d.Accept(writer2);

  // // Output {"project":"rapidjson","stars":11}
  // std::cout << buffer.GetString() << std::endl;


  // // this->server->send(s.GetString());
  std::cout << card->getName() << std::endl;
  std::cout << card->toJson() << std::endl;

}