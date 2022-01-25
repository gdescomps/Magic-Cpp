#pragma once

#include <memory> 
#include <vector>
#include <functional>

#include "Card.hpp"

class CardRegistry {
  using Ctor = std::function<Card*()>;
    
public:
    
  static CardRegistry& getInst() {
    static CardRegistry inst;
    return inst;
  }
  
  int registerCard(Ctor c) {
    factories.push_back(c);
    return factories.size() - 1;
  }
  
  template<class C>
  int registerCard() {
    factories.push_back([]() { return new C(); });
    return factories.size() - 1;
  }
  
  template<class C>
  std::unique_ptr<C> create(int id) {
    Card* card = factories.at(id)();
    C* res = dynamic_cast<C*>(card);
    if(res == nullptr) {
      delete card; 
      throw std::runtime_error("invalid card creation type");
    }
    return std::unique_ptr<C>(res);
  }
  
private:
  CardRegistry() = default;
  CardRegistry(CardRegistry const&) = delete;
  CardRegistry& operator=(CardRegistry const&) = delete;
  std::vector<Ctor> factories;
};
