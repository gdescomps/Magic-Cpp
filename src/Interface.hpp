#pragma once

#include <curses.h>
#include <string>
#include <vector>
#include <algorithm>

#include "Card.hpp"
#include "Creature.hpp"
#include "Land.hpp"

class MenuEntry {

public:
  enum class State { NORMAL, SELECTED, DISABLED };  

  MenuEntry(std::string text, State state)
   : text(text), state(state)
  {}
  
  MenuEntry(char const* text)
   : text(text), state(State::NORMAL)
  {}
  
  std::string text;
  State state; 
};


class Interface {
public:
  Interface();
  ~Interface();

  void showWelcome();
  
  template<class C>
  void showCard(C const* c);
  
  template<class C>
  void showCards(std::string const& msg, std::vector<C*> const& cards);

  template<class C>
  C* selectCard(std::string const& msg, std::vector<C*> const& cards);

  template<class C>
  std::vector<C*> selectCards(std::string const& msg, std::vector<C*> const& cards);
  
  void hideAll();

  void tell(std::string const& msg);

  std::string prompt(std::string const& msg);

  bool promptYesNo(std::string const& msg);

  int showMenu(std::string const& msg, std::vector<MenuEntry> choices);

  std::vector<bool> showMenuMultiple(std::string const& msg, std::vector<MenuEntry> choices);

private:
  WINDOW* wheader;
  WINDOW* wprompt;
  WINDOW* wmain;
  void draw();
  
  int cardW = 25;
  int cardH = 15;

  template<class C>
  void drawCard(WINDOW* win, C const* card);
  
  int getFgColor(Mana m) const;
  int getBgColor(Mana m) const;
};

template<class C>
inline C* Interface::selectCard(std::string const& msg, std::vector<C*> const& cards) {
  int selected = -1;
  
  keypad(stdscr, TRUE);
  noecho();
  int cur = 0;
  bool cont = true;

  do {
    hideAll();

    mvwprintw(wmain, 0, 0, msg.c_str());
    std::string line(msg.size(), '-');
    mvwprintw(wmain, 1, 0, line.c_str());

    for(size_t i = 0; i < cards.size(); i++) {
      std::string box = selected == (int)i ? "[+] " : "[ ] ";
      mvwprintw(wmain, i + 2, 1, box.c_str());
      wprintw(wmain, cards[i]->getName().c_str());
    }
    
    wmove(wmain, cur + 2, 2); // move to first box
    showCard(cards[cur]);
    wrefresh(wmain);
    
    int key = getch();
    if(key == '\n') cont = false;
    else if(key == KEY_UP) { cur = cur == 0 ? (int)cards.size() - 1 : cur - 1; }
    else if(key == KEY_DOWN) { cur = (cur + 1) % (int)cards.size(); }
    else if(key == ' ') { selected = cur == selected ? -1 : cur; }
  }
  while(cont);
  
  echo();
  hideAll();

  return selected == -1 ? nullptr : cards[selected];
}

template<class C>
inline std::vector<C*> Interface::selectCards(std::string const& msg, std::vector<C*> const& cards) {
  std::vector<bool> selected(cards.size());
  std::fill(selected.begin(), selected.end(), false);

  keypad(stdscr, TRUE);
  noecho();
  int cur = 0;
  bool cont = true;

  do {
    hideAll();

    mvwprintw(wmain, 0, 0, msg.c_str());
    std::string line(msg.size(), '-');
    mvwprintw(wmain, 1, 0, line.c_str());

    for(size_t i = 0; i < cards.size(); i++) {
      std::string box = selected[i] ? "[*] " : "[ ] ";
      mvwprintw(wmain, i + 2, 1, box.c_str());
      wprintw(wmain, cards[i]->getName().c_str());
    }
    
    wmove(wmain, cur + 2, 2); // move to first box
    showCard(cards[cur]);
    wrefresh(wmain);
    
    int key = getch();
    if(key == '\n') cont = false;
    else if(key == KEY_UP) { cur = cur == 0 ? (int)cards.size() - 1 : cur - 1; }
    else if(key == KEY_DOWN) { cur = (cur + 1) % (int)cards.size(); }
    else if(key == ' ') { selected[cur] = !selected[cur]; }
  }
  while(cont);
  
  echo();
  hideAll();

  std::vector<C*> res;
  for(size_t i = 0; i < selected.size(); i++) {
    if(selected[i]) res.push_back(cards[i]);
  }

  return res;
}

template<class C>
inline void Interface::showCard(C const* card) {
  int cy = (getmaxy(wmain) - cardH) / 2;
  int cx = (getmaxx(wmain) - cardW) / 2;
  WINDOW* wout = derwin(wmain, cardH, cardW, cy, cx);
  WINDOW* wrect = derwin(wout, cardH - 2, cardW - 2, 1, 1);
  
  int col = getFgColor(card->getMana());
  wattron(wout, COLOR_PAIR(col));
  wborder(wout, 0, 0, 0, 0, 0, 0, 0, 0);
  wattroff(wout, COLOR_PAIR(col));
  
  drawCard(wrect, card);

  wrefresh(wout);
  wrefresh(wrect);
  delwin(wout);
  delwin(wrect);
}

template<class C>
inline void Interface::showCards(std::string const& msg, std::vector<C*> const& cards) {
  keypad(stdscr, TRUE);
  noecho();
  int cur = 0;
  bool cont = true;

  do {
    hideAll();

    mvwprintw(wmain, 0, 0, msg.c_str());
    std::string line(msg.size(), '-');
    mvwprintw(wmain, 1, 0, line.c_str());

    for(size_t i = 0; i < cards.size(); i++) {
      std::string box = cur == (int)i ? "[-] " : "[ ] ";
      mvwprintw(wmain, i + 2, 1, box.c_str());
      wprintw(wmain, cards[i]->getName().c_str());
    }
    
    wmove(wmain, cur + 2, 2); // move to first box
    showCard(cards[cur]);
    wrefresh(wmain);
    
    int key = getch();
    if(key == '\n') cont = false;
    else if(key == KEY_UP) { cur = cur == 0 ? (int)cards.size() - 1 : cur - 1; }
    else if(key == KEY_DOWN) { cur = (cur + 1) % (int)cards.size(); }
  }
  while(cont);
  
  echo();
  hideAll();
}

template<> void Interface::drawCard(WINDOW* win, Card const* card);
template<> void Interface::drawCard(WINDOW* win, Land const* land);
template<> void Interface::drawCard(WINDOW* win, Creature const* creature);
