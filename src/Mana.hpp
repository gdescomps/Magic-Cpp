#pragma once

#include <algorithm>
#include <array>
#include <stddef.h>

enum class Mana { WHITE, BLUE, BLACK, RED, GREEN };

inline std::string manaToString(Mana m) {
  constexpr std::array lookup = {
    "White", "Blue", "Black", "Red", "Green"
  };
  return lookup[(size_t)m];
}

class ManaCost {
public:
  ManaCost(): anyCost(0) {
    std::ranges::fill(cost, 0);  
  }

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
   * Wether the mana matches exactly the requirement.
   *  - each mana must be greater than or equal to the corresponding requirement
   *  - the remaining mana must match the 'any' requirement
   *  - the 'any' mana of this ManaCost is ignored  
   */
  bool matches(ManaCost const& requirement) const {
    int remain = 0;
    for(size_t i = 0; i < cost.size(); i++) {
      if(requirement.cost[i] > cost[i]) return false;
      remain += cost[i] - requirement.cost[i];
    }
    return remain == requirement.getAny();
  }

  /** 
   * Wether the mana meet at least the requirement.
   *  - each mana must be greater than or equal to the corresponding requirement
   *  - the remaining mana must meet at least the 'any' requirement
   *  - the 'any' mana of this ManaCost is ignored  
   */
  bool enough(ManaCost const& requirement) const {
    int remain = 0;
    for(size_t i = 0; i < cost.size(); i++) {
      if(requirement.cost[i] > cost[i]) return false;
      remain += cost[i] - requirement.cost[i];
    }
    return remain >= requirement.getAny();
  }

private:
  std::array<int, 5> cost;
  int anyCost;
};

