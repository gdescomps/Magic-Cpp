#include "Interface.hpp"

#include <ncurses.h>
#include <iostream>
#include <string>

#include "Creature.hpp"

void hcwprintw(WINDOW* win, int y, std::string txt) {
  int x = (getmaxx(win) - txt.size()) / 2;
  mvwprintw(win, y, x, txt.c_str());
}

Interface::Interface() {
  initscr();

  int w = getmaxx(stdscr);
  int h = getmaxy(stdscr);

  wheader = newwin(3, w, 0, 0);
  wprompt = newwin(2, w, h - 2, 0);
  wmain = newwin(h - 3 - 2, w, 3, 0);

  hcwprintw(wheader, 1, "MAGIC");
  wborder(wheader, 0, 0, 0, 0, 0, 0, 0, 0);
  whline(wprompt, 0, w);

  refresh();
  wrefresh(wheader);
  wrefresh(wprompt);
  wrefresh(wmain);
}

Interface::~Interface() {
  endwin();
}

void Interface::showWelcome() {
}


void Interface::drawLandCard(WINDOW* wrect, Land const* land) {
  int w = getmaxx(wrect);
  int h = getmaxy(wrect);
 
  // card header
  wattron(wrect, A_BOLD);
  hcwprintw(wrect, 0, land->getName().c_str());
  wattroff(wrect, A_BOLD);
  mvwhline(wrect, 1, 0, '-', w);
  mvwprintw(wrect, 2, 1, land->getType().c_str());
}

void Interface::drawCreatureCard(WINDOW* wrect, Creature const* creature) {
  int w = getmaxx(wrect);
  int h = getmaxy(wrect);
  
  // card header
  wattron(wrect, A_BOLD);
  hcwprintw(wrect, 0, creature->getName().c_str());
  wattroff(wrect, A_BOLD);
  mvwhline(wrect, 1, 0, '-', w);
  mvwprintw(wrect, 2, 1, creature->getType().c_str());
  
  // power / toughness  
  mvwhline(wrect, h - 2, 0, '-', w);
  auto stats = std::to_string(creature->getPower()) + " / " + std::to_string(creature->getToughness());
  mvwprintw(wrect, h - 1, w - stats.size() - 1, stats.c_str());
}

void Interface::showCard(Card const* card) {
  int w = 30;
  int h = 20;
  int cy = (getmaxy(wmain) - h) / 2;
  int cx = (getmaxx(wmain) - w) / 2;
  WINDOW* wout = derwin(wmain, h, w, cy, cx);
  WINDOW* wrect = derwin(wout, h - 2, w - 2, 1, 1);
  wborder(wout, 0, 0, 0, 0, 0, 0, 0, 0);
  
  drawCard(wrect, card);

  wrefresh(wout);
  wrefresh(wrect);
  delwin(wout);
  delwin(wrect);
}

void Interface::showCards(std::vector<Card*> const& cards) {
  int w = 20;
  int h = 15;
  int cy = (getmaxy(wmain) - h) / 2;
  int gap = (getmaxx(wmain) - w * cards.size()) / (cards.size() + 1);
  int offset = gap;

  for(auto card : cards) {
    WINDOW* wout = derwin(wmain, h, w, cy, offset);
    WINDOW* wrect = derwin(wout, h - 2, w - 2, 1, 1);
    wborder(wout, 0, 0, 0, 0, 0, 0, 0, 0);
    
    drawCard(wrect, card);
  
    wrefresh(wout);
    wrefresh(wrect);
    delwin(wout);
    delwin(wrect);
    offset += gap + w;
  } 
}

void Interface::drawCard(WINDOW* wrect, Card const* card) {  
  // And now, the ugly part
  if(dynamic_cast<Creature const*>(card)) { 
    auto creature = dynamic_cast<Creature const*>(card);
    drawCreatureCard(wrect, creature);
  }
  else if(dynamic_cast<Land const*>(card)) {
    auto land = dynamic_cast<Land const*>(card);
    drawLandCard(wrect, land);
  }
  else {
    wattron(wrect, A_BOLD);
    hcwprintw(wrect, 0, "Unknown Card");
    wattroff(wrect, A_BOLD);
  }
}

void Interface::hideAll() {
  werase(wmain);
  wrefresh(wmain);
}

std::string Interface::prompt(std::string const& msg) {
  int w = getmaxx(wprompt);
  WINDOW* win = derwin(wprompt, 1, w, 1, 0);
  mvwprintw(win, 0, 0, msg.c_str());
  wprintw(win, " > ");
  wrefresh(win);
  int count = w - 1 - getcurx(win);
  char buf[count + 1];
  wgetnstr(win, buf, count);

  werase(win);
  delwin(win);
  wrefresh(wprompt);

  return buf;
}