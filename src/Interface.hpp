#pragma once

#include <curses.h>
#include <string>
#include <vector>
#include <algorithm>

#include "Card.hpp"
#include "Creature.hpp"
#include "Land.hpp"

class Interface {
public:
  Interface();
  ~Interface();

  void showWelcome();
  
  template<class C>
  void showCard(C const* c);
  
  template<class C>
  void showCards(std::vector<C*> const& cards);

  template<class C>
  C* selectCard(std::string const& msg, std::vector<C*> const& cards);

  template<class C>
  std::vector<C*> selectCards(std::string const& msg, std::vector<C*> const& cards);
  
  void hideAll();

  void tell(std::string const& msg);

  std::string prompt(std::string const& msg);

  bool promptYesNo(std::string const& msg);

  int showMenu(std::string const& msg, std::vector<std::string> choices);

  std::vector<bool> showMenuMultiple(std::string const& msg, std::vector<std::string> choices);

private:
  WINDOW* wheader;
  WINDOW* wprompt;
  WINDOW* wmain;
  void draw();
  
  int cardW = 20;
  int cardH = 10;

  template<class C>
  void drawCard(WINDOW* win, C const* card);
};

template<class C>
inline C* Interface::selectCard(std::string const& msg, std::vector<C*> const& cards) {
  std::vector<std::string> names(cards.size());
  for(size_t i = 0; i < cards.size(); i++) {
    names[i] = cards[i]->getName();
  }

  C* res = nullptr;
  bool cont = true;

  do {
    auto selected = showMenu(msg, names);
    res = selected == -1 ? nullptr : cards[selected];
        
    if(res == nullptr) {
      cont = !promptYesNo("Select nothing?");
    }
    else {
      showCard<C>(res);
      cont = !promptYesNo("Is this OK?");
    }
  }
  while(cont);

  return res;
}

template<class C>
inline std::vector<C*> Interface::selectCards(std::string const& msg, std::vector<C*> const& cards) {
  std::vector<std::string> names(cards.size());
  for(size_t i = 0; i < cards.size(); i++) {
    names[i] = cards[i]->getName();
  }

  std::vector<C*> res;
  bool cont = true;

  do {
    auto selected = showMenuMultiple(msg, names);
    res.clear();  
    for(size_t i = 0; i < selected.size(); i++) {
      if(selected[i]) {
        res.push_back(cards[i]);
      }
    }
    
    if(cards.size() == 0) {
      cont = !promptYesNo("Select nothing?");
    }
    else {
      showCards<C>(res);
      cont = !promptYesNo("Is this OK?");
    }
  }
  while(cont);

  return res;
}

template<class C>
inline void Interface::showCard(C const* card) {
  int cy = (getmaxy(wmain) - cardH) / 2;
  int cx = (getmaxx(wmain) - cardW) / 2;
  WINDOW* wout = derwin(wmain, cardH, cardW, cy, cx);
  WINDOW* wrect = derwin(wout, cardH - 2, cardW - 2, 1, 1);
  wborder(wout, 0, 0, 0, 0, 0, 0, 0, 0);
  
  drawCard(wrect, card);

  wrefresh(wout);
  wrefresh(wrect);
  delwin(wout);
  delwin(wrect);
}

template<class C>
inline void Interface::showCards(std::vector<C*> const& cards) {
  int cy = (getmaxy(wmain) - cardH) / 2;
  int gap = (getmaxx(wmain) - cardW * cards.size()) / (cards.size() + 1);
  int offset = gap;

  for(auto card : cards) {
    WINDOW* wout = derwin(wmain, cardH, cardW, cy, offset);
    WINDOW* wrect = derwin(wout, cardH - 2, cardW - 2, 1, 1);
    wborder(wout, 0, 0, 0, 0, 0, 0, 0, 0);
    
    drawCard(wrect, card);
  
    wrefresh(wout);
    wrefresh(wrect);
    delwin(wout);
    delwin(wrect);
    offset += gap + cardW;
  }
}

template<> void Interface::drawCard(WINDOW* win, Card const* card);
template<> void Interface::drawCard(WINDOW* win, Land const* land);
template<> void Interface::drawCard(WINDOW* win, Creature const* creature);
