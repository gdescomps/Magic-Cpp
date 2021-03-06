#include "Interface.hpp"

#include <ncurses.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <numeric>

#include "Creature.hpp"
#include "Ability.hpp"

void hcwprintw(WINDOW* win, int y, std::string txt) {
  int x = (getmaxx(win) - txt.size()) / 2;
  mvwprintw(win, y, x, txt.c_str());
}

int selectedIndex(std::vector<MenuEntry> const& entries) {
  auto it = std::ranges::find_if(entries, [] (MenuEntry const& e) {
    return e.state == MenuEntry::State::SELECTED;
  });
  return it == entries.end() ? -1 : it - entries.begin();
}

void selectNext(std::vector<MenuEntry>& entries) {
  auto sel = std::ranges::find_if(entries, [] (MenuEntry const& e) {
    return e.state == MenuEntry::State::SELECTED;
  });

  // nothing is selected, select first selectable
  if(sel == entries.end()) {
    auto it = std::ranges::find_if(entries, [] (MenuEntry const& e) {
      return e.state != MenuEntry::State::DISABLED;
    });

    if(it != entries.end()) {
      it->state = MenuEntry::State::SELECTED;
    }
  }
  
  // select next with wrap-around
  else {
    sel->state = MenuEntry::State::NORMAL;

    auto it = std::find_if(sel + 1, entries.end(), [] (MenuEntry const& e) {
      return e.state == MenuEntry::State::NORMAL;
    });

    if(it == entries.end()) {
      it = std::ranges::find_if(entries, [] (MenuEntry const& e) {
        return e.state == MenuEntry::State::NORMAL;
      });
    }
    
    it->state = MenuEntry::State::SELECTED;
  }
}

void selectPrevious(std::vector<MenuEntry>& entries) {
  auto sel = std::ranges::find_if(entries, [] (MenuEntry const& e) {
    return e.state == MenuEntry::State::SELECTED;
  });

  // nothing is selected, select first selectable
  if(sel == entries.end()) {
    auto it = std::ranges::find_if(entries, [] (MenuEntry const& e) {
      return e.state != MenuEntry::State::DISABLED;
    });

    if(it != entries.end()) {
      it->state = MenuEntry::State::SELECTED;
    }
  }
 
  // select previous with wrap-around
  else {
    sel->state = MenuEntry::State::NORMAL;

    auto it = std::find_if(std::reverse_iterator(sel), entries.rend(), [] (MenuEntry const& e) {
      return e.state == MenuEntry::State::NORMAL;
    });

    if(it == entries.rend()) {
      it = std::find_if(entries.rbegin(), entries.rend(), [] (MenuEntry const& e) {
        return e.state == MenuEntry::State::NORMAL;
      });
    }
    
    it->state = MenuEntry::State::SELECTED;
  }
}

Interface::Interface() {
  initscr();
  clear();
  cbreak();

  start_color();
  init_pair(1, COLOR_BLACK, COLOR_YELLOW); // White mana
  init_pair(2, COLOR_WHITE, COLOR_BLUE); // Blue mana
  init_pair(3, COLOR_BLACK, COLOR_MAGENTA); // Black mana
  init_pair(4, COLOR_WHITE, COLOR_RED); // Red mana
  init_pair(5, COLOR_BLACK, COLOR_GREEN); // Green mana

  init_pair(6, COLOR_YELLOW, COLOR_BLACK); // White mana
  init_pair(7, COLOR_BLUE, COLOR_BLACK); // Blue mana
  init_pair(8, COLOR_MAGENTA, COLOR_BLACK); // Black mana
  init_pair(9, COLOR_RED, COLOR_BLACK); // Red mana
  init_pair(10, COLOR_GREEN, COLOR_BLACK); // Green mana

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

int Interface::showMenu(std::string const& msg, std::vector<MenuEntry> choices) {
  keypad(stdscr, TRUE);
  noecho();
  bool cont = true;

  if(selectedIndex(choices) == -1)
    selectNext(choices);

  do {
    hideAll();

    mvwprintw(wmain, 0, 0, msg.c_str());
    std::string line(msg.size(), '-');
    mvwprintw(wmain, 1, 0, line.c_str());

    for(size_t i = 0; i < choices.size(); i++) {
      std::string box = choices[i].state == MenuEntry::State::NORMAL ? "[ ] "
                      : choices[i].state == MenuEntry::State::DISABLED ?  "    "
                      : "[+] ";
      mvwprintw(wmain, i + 2, 1, box.c_str());
      wprintw(wmain, choices[i].text.c_str());
    }
    
    int cur = selectedIndex(choices);
    wmove(wmain, cur + 2, 2); // move to first box
    wrefresh(wmain);
    
    int key = getch();
    if(key == '\n') cont = false;
    else if(key == KEY_UP) { selectPrevious(choices); }
    else if(key == KEY_DOWN) { selectNext(choices); }
    else if(key == ' ') { 
      int i = selectedIndex(choices);
      if(i != -1) choices[i].state = MenuEntry::State::NORMAL; 
      choices[cur].state = MenuEntry::State::SELECTED;
    }
  }
  while(cont);
  
  echo();
  hideAll();

  return selectedIndex(choices);
}

std::vector<bool> Interface::showMenuMultiple(std::string const& msg, std::vector<MenuEntry> choices) {
  keypad(stdscr, TRUE);
  noecho();
  int cur = 0;
  bool cont = true;

  do {
    hideAll();
    
    mvwprintw(wmain, 0, 0, msg.c_str());
    std::string line(msg.size(), '-');
    mvwprintw(wmain, 1, 0, line.c_str());

    for(size_t i = 0; i < choices.size(); i++) {
      std::string box = choices[i].state == MenuEntry::State::NORMAL ? "[ ] "
                      : choices[i].state == MenuEntry::State::DISABLED ?  "    "
                      : "[*] ";
      mvwprintw(wmain, i + 2, 1, box.c_str());
      wprintw(wmain, choices[i].text.c_str());
    }
    
    wmove(wmain, cur + 2, 2); // move to first box
    wrefresh(wmain);
    
    int key = getch();
    if(key == '\n') cont = false;
    else if(key == KEY_UP) { cur = cur == 0 ? (int)choices.size() - 1 : cur - 1; }
    else if(key == KEY_DOWN) { cur = (cur + 1) % (int)choices.size(); }
    else if(key == ' ') { 
      auto s = choices[cur].state;
      choices[cur].state = s == MenuEntry::State::SELECTED ? MenuEntry::State::NORMAL : MenuEntry::State::SELECTED;
    }
  }
  while(cont);
  
  echo();
  hideAll();

  std::vector<bool> res(choices.size());
  for(size_t i = 0; i < choices.size(); i++) {
    res[i] = (choices[i].state == MenuEntry::State::SELECTED);
  }
  return res;
}

int Interface::drawCardHeader(WINDOW* wrect, Card const* card) {
  int w = getmaxx(wrect);
 
  // mana cost
  auto cost = card->getCost();
  int off = w;
  for(size_t i = 0; i < 5; i++) {
    Mana m = (Mana)i;
    int c = cost.get(m);
    if(c != 0) {
      wattron(wrect, COLOR_PAIR(getBgColor(m)));
      mvwaddch(wrect, 0, --off, c + '0'); 
      wattroff(wrect, COLOR_PAIR(getBgColor(m)));
    }
  }
  if(cost.getAny() != 0) {
    mvwaddch(wrect, 0, --off, cost.getAny() + '0'); 
  }

  // card name, type, description
  wattron(wrect, A_BOLD);
  hcwprintw(wrect, 0, card->getName().c_str());
  wattroff(wrect, A_BOLD);
  mvwhline(wrect, 1, 0, '-', w);
  mvwprintw(wrect, 2, 0, card->getType().c_str());
  wattron(wrect, A_ITALIC);
  int count = writeText(wrect, 3, 0, card->getDesc(), w);
  wattroff(wrect, A_ITALIC);
  return count + 3;
}

void Interface::drawCardFooter(WINDOW* wrect, Card const* card) {
  int w = getmaxx(wrect);
  int h = getmaxy(wrect);

  mvwhline(wrect, h - 2, 0, '-', w);
  auto s = Card::stateToString(card->getState()) + (card->isTapped() ? " (tapped)" : "");
  mvwprintw(wrect, h - 1, 0, s.c_str());
}

template<>
void Interface::drawCard(WINDOW* wrect, Land const* land) {
  drawCardHeader(wrect, land);
  drawCardFooter(wrect, land);
}

template<>
void Interface::drawCard(WINDOW* wrect, Creature const* creature) {
  int w = getmaxx(wrect);
  int h = getmaxy(wrect);
  
  int off = 1 + drawCardHeader(wrect, creature);
  
  // abilities
  if(creature->getAbilities().size() <= 3) {
    for(auto ability : creature->getAbilities()) {
      auto name = ability->getName() + ": ";
      auto desc = ability->getDesc();
      wattron(wrect, A_BOLD);
      mvwprintw(wrect, off, 0, name.c_str());
      wattroff(wrect, A_BOLD);
      off += 1 + writeText(wrect, off, name.size(), desc.c_str(), w);
    }
  }
  else {
    wattron(wrect, A_BOLD);
    mvwprintw(wrect, off, 0, "Abilities: ");
    wattroff(wrect, A_BOLD);
    auto const& a = creature->getAbilities();
    auto msg = std::accumulate(++a.begin(), a.end(), a[0]->getName(), [] (std::string acc, Ability* a) {
      return acc + ", " + a->getName();
    });
    writeText(wrect, off, 11, msg.c_str(), w);
  }

  // power / toughness
  drawCardFooter(wrect, creature);
  auto stats = std::to_string(creature->getPower()) + " / " + std::to_string(creature->getToughness());
  mvwprintw(wrect, h - 1, w - stats.size(), stats.c_str());
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
    drawCardHeader(wrect, card);
    wattron(wrect, A_BOLD);
    hcwprintw(wrect, getmaxy(wrect) - 1, "Unknown Card");
    wattroff(wrect, A_BOLD);
    drawCardFooter(wrect, card);
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

int Interface::writeText(WINDOW* win, int y, int x, std::string msg, int maxW) {
  int i = 0;
  while(msg.size()) {
    std::string row = msg.substr(0, maxW - x);
    msg = msg.substr(row.size(), msg.size());
    if(msg.size() && !std::isspace(msg[0])) {
      auto it = std::find_if(row.rbegin(), row.rend(), [] (char c) { return std::isspace(c); });
      if(it != row.rend()) {
        msg.insert(msg.begin(), it.base(), row.end());        
        row.erase(it.base(), row.end());
      }
    }
    msg.erase(msg.begin(), std::ranges::find_if(msg, [] (char c) { return !std::isspace(c); }));
    wmove(win, y, x);
    x = 0;
    wprintw(win, row.c_str());
    y++;
    i++;
  }
  return i;
}

int Interface::getFgColor(Mana m) const {
  return getBgColor(m) + 5; 
}

int Interface::getBgColor(Mana m) const {
  return (int)m + 1;
}
