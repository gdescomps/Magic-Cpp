#pragma once

#include <curses.h>
#include <string>
#include <vector>

#include "Card.hpp"
#include "Creature.hpp"
#include "Land.hpp"

class Interface {
public:
  Interface();
  ~Interface();

  void showWelcome();
  void showCard(Card const* c);
  void showCards(std::vector<Card*> const& cards);
  void hideAll();
  std::string prompt(std::string const& msg);

private:
  WINDOW* wheader;
  WINDOW* wprompt;
  WINDOW* wmain;
  void draw();
  
  void drawCard(WINDOW* win, Card const* card);
  void drawLandCard(WINDOW* win, Land const* land);
  void drawCreatureCard(WINDOW* win, Creature const* creature);
};

