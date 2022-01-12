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
  clear();
  cbreak();

  start_color();
  init_pair(1, COLOR_BLACK, COLOR_WHITE); // White mana
  init_pair(2, COLOR_WHITE, COLOR_BLUE); // Blue mana
  init_pair(3, COLOR_WHITE, COLOR_BLACK); // Black mana
  init_pair(4, COLOR_WHITE, COLOR_RED); // Red mana
  init_pair(5, COLOR_WHITE, COLOR_GREEN); // Green mana

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

int Interface::showMenu(std::string const& msg, std::vector<std::string> choices) {
  mvwprintw(wmain, 0, 0, msg.c_str());
  std::string line(msg.size(), '-');
  mvwprintw(wmain, 1, 0, line.c_str());
  
  int selected = -1;
  
  keypad(stdscr, TRUE);
  noecho();
  int i = 0;
  bool cont = true;

  do {
    hideAll();
    for(size_t i = 0; i < choices.size(); i++) {
      std::string box = selected == (int)i ? "[+] " : "[ ] ";
      mvwprintw(wmain, i + 2, 1, box.c_str());
      wprintw(wmain, choices[i].c_str());
    }
    
    wmove(wmain, i + 2, 2); // move to first box
    wrefresh(wmain);
    
    int key = getch();
    if(key == '\n') cont = false;
    else if(key == KEY_UP) { i = std::max(i - 1, 0); }
    else if(key == KEY_DOWN) { i = std::min(i + 1, (int)choices.size() - 1); }
    else if(key == ' ') { selected = i == selected ? -1 : i; }
  }
  while(cont);
  
  echo();
  hideAll();

  return selected;
}

std::vector<bool> Interface::showMenuMultiple(std::string const& msg, std::vector<std::string> choices) {
  mvwprintw(wmain, 0, 0, msg.c_str());
  std::string line(msg.size(), '-');
  mvwprintw(wmain, 1, 0, line.c_str());
  
  std::vector<bool> selected(choices.size());
  std::fill(selected.begin(), selected.end(), false);
  
  keypad(stdscr, TRUE);
  noecho();
  int i = 0;
  bool cont = true;

  do {
    hideAll();
    for(size_t i = 0; i < choices.size(); i++) {
      std::string box = selected[i] ? "[*] " : "[ ] ";
      mvwprintw(wmain, i + 2, 1, box.c_str());
      wprintw(wmain, choices[i].c_str());
    }
    
    wmove(wmain, i + 2, 2); // move to first box
    wrefresh(wmain);
    
    int key = getch();
    if(key == '\n') cont = false;
    else if(key == KEY_UP) { i = std::max(i - 1, 0); }
    else if(key == KEY_DOWN) { i = std::min(i + 1, (int)choices.size() - 1); }
    else if(key == ' ') { selected[i] = !selected[i]; }
  }
  while(cont);
  
  echo();
  hideAll();

  return selected;
}

template<>
void Interface::drawCard(WINDOW* wrect, Land const* land) {
  int w = getmaxx(wrect);
 
  // card header
  wattron(wrect, A_BOLD);
  hcwprintw(wrect, 0, land->getName().c_str());
  wattroff(wrect, A_BOLD);
  mvwhline(wrect, 1, 0, '-', w);
  mvwprintw(wrect, 2, 1, land->getType().c_str());
}

template<>
void Interface::drawCard(WINDOW* wrect, Creature const* creature) {
  int w = getmaxx(wrect);
  int h = getmaxy(wrect);
  
  // card header
  wattron(wrect, A_BOLD);
  hcwprintw(wrect, 0, creature->getName().c_str());
  wattroff(wrect, A_BOLD);
  mvwhline(wrect, 1, 0, '-', w);
  mvwprintw(wrect, 2, 1, creature->getType().c_str());

  // mana cost
  auto cost = creature->getCost();
  int off = w;
  for(size_t i = 0; i < 5; i++) {
    Mana m = (Mana)i;
    int c = cost.get(m);
    if(c != 0) {
      tell(std::to_string(i));
      wattron(wrect, COLOR_PAIR(i + 1));
      mvwaddch(wrect, 0, --off, c + '0'); 
      wattroff(wrect, COLOR_PAIR(i + 1));
    }
  }
  if(cost.getAny() != 0) {
    mvwaddch(wrect, 0, --off, cost.getAny() + '0'); 
  }

  // power / toughness  
  mvwhline(wrect, h - 2, 0, '-', w);
  auto stats = std::to_string(creature->getPower()) + " / " + std::to_string(creature->getToughness());
  mvwprintw(wrect, h - 1, w - stats.size() - 1, stats.c_str());
}

template<>
void Interface::drawCard(WINDOW* wrect, Card const* card) {  
  if(dynamic_cast<Creature const*>(card)) { 
    auto creature = dynamic_cast<Creature const*>(card);
    drawCard(wrect, creature);
  }
  else if(dynamic_cast<Land const*>(card)) {
    auto land = dynamic_cast<Land const*>(card);
    drawCard(wrect, land);
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

void Interface::tell(std::string const& msg) {
  int w = getmaxx(wprompt);
  WINDOW* win = derwin(wprompt, 1, w, 1, 0);
  mvwprintw(win, 0, 0, msg.c_str());
  wprintw(win, " (Press any key to continue) > ");
  wrefresh(win);
  wgetch(win);
  werase(win);
  delwin(win);
  wrefresh(wprompt);
}

bool Interface::promptYesNo(std::string const& msg) {
  std::string resp = prompt(msg + " (Y/n)");
  std::ranges::transform(resp, resp.begin(), [] (char c) { return std::tolower(c); });
  return (resp == "yes" || resp == "y" || resp == "");
}
