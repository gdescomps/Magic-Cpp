#pragma once

#include <curses.h>
#include <string>
#include <vector>
#include <algorithm>

#include "Card.hpp"
#include "Creature.hpp"
#include "Land.hpp"

/** A menu entry. 
    Can be created from a string with default state of 'NORMAL', or also provide a state. */
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

/** The game terminal-based user interface.
    It uses the library ncurse to manage drawing on the terminal.
    The ui is composed of a header bar (title), a main screen and a prompt. */
class UserInterface {
public:
  UserInterface();
  ~UserInterface();

  void showWelcome();
  
  /** Show a card details centered on the screen.
      Note: call hideAll() to hide */  
  template<class C>
  void showCard(C const* c);
  
  /** Show cards details centered on the screen.
      Note: call hideAll() to hide */
  template<class C>
  void showCards(std::string const& msg, std::vector<C*> const& cards);

  /** Show a selector menu to select a single card in a list.
      @param msg a message to prompt the user.
      @param cards a list of cards to choose from.
      @return the selected card or nullptr if selection was cancelled. */  
  template<class C>
  C* selectCard(std::string const& msg, std::vector<C*> const& cards);

  /** Show a selector menu to select a single card in a list.
      @param msg a message to prompt the user.
      @param cards a list of cards to choose from.
      @return a possibly empty list of selected cards. */  
  template<class C>
  std::vector<C*> selectCards(std::string const& msg, std::vector<C*> const& cards);
  
  /** Hide content drawn on the main screen (not the header nor the prompt). */
  void hideAll();
  
  /** Tell a message to the user. */
  void tell(std::string const& msg);

  std::string prompt(std::string const& msg);

  /** Ask a yes / no question to the user. 
      @param msg the question to ask.
      @return true if answer was 'yes', false if answer was 'no'. */
  bool promptYesNo(std::string const& msg);

  /** Show a menu. 
      @param msg the displayed menu name.
      @param choices the different menu entries.
      @return the selected entry index in choices, or -1 if cancelled. */
  int showMenu(std::string const& msg, std::vector<MenuEntry> choices);

  /** Show a menu with multiple selection. 
      @param msg the displayed menu name.
      @param choices the different menu entries.
      @return a vector of selected, i.e. whether the corresponding choice was selected. */
  std::vector<bool> showMenuMultiple(std::string const& msg, std::vector<MenuEntry> choices);

private:
  WINDOW* wheader;
  WINDOW* wprompt;
  WINDOW* wmain;

  void draw();
  void writeText(WINDOW* win, int y, int x, std::string text, int maxW);
  
  int cardW = 35;
  int cardH = 20;

  template<class C>
  void drawCard(WINDOW* win, C const* card);
  
  void drawCardHeader(WINDOW* win, Card const* card);
  
  int getFgColor(Mana m) const;
  int getBgColor(Mana m) const;
};

template<class C>
inline C* UserInterface::selectCard(std::string const& msg, std::vector<C*> const& cards) {
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
inline std::vector<C*> UserInterface::selectCards(std::string const& msg, std::vector<C*> const& cards) {
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
inline void UserInterface::showCard(C const* card) {
  int cy = (getmaxy(wmain) - cardH) / 2;
  int cx = (getmaxx(wmain) - cardW) / 2;
  WINDOW* wout = derwin(wmain, cardH, cardW, cy, cx);
  WINDOW* wrect = derwin(wout, cardH - 2, cardW - 4, 1, 2);
  
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
inline void UserInterface::showCards(std::string const& msg, std::vector<C*> const& cards) {
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

template<> void UserInterface::drawCard(WINDOW* win, Card const* card);
template<> void UserInterface::drawCard(WINDOW* win, Land const* land);
template<> void UserInterface::drawCard(WINDOW* win, Creature const* creature);
