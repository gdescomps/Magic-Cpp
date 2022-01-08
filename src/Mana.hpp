#pragma once

#include <array>
#include <stddef.h>

enum class Mana { WHITE, BLUE, BLACK, RED, GREEN };

class ManaCost {
public:
  ManaCost() {}

  void add(Mana mana) {
    cost[(size_t)mana]++;
  }
  
  void set(Mana mana, int amount) {
    cost[(size_t)mana] = amount;
  }
  
  int get(Mana mana) const {
    return cost[(size_t)mana];
  }
  
  void addAny() {
    anyCost++;
  }

  void setAny(int amount) {
    anyCost = amount;
  }

  int getAny() const {
    return anyCost;
  }
  
  /** 
   * Wether the mana matches the requirement.
   * All Mana 
   */
  bool matches(ManaCost const& requirement) const {
    int remain = 0;
    for(size_t i = 0; i < cost.size() - 2; i++) {
      if(requirement.cost[i] > cost[i]) return false;
      remain += cost[i] - requirement.cost[i];
    }
    return remain == requirement.getAny();
  }

private:
  std::array<int, 5> cost;
  int anyCost;
};

