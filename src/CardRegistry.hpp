#pragma once

#include <memory>
#include <unordered_map>
#include <functional>
#include <iostream>

#include "Card.hpp"

class CardRegistry {
  using Ctor = std::function<Card*()>;

public:

  static void registerAllCards();

  static CardRegistry& getInst() {
    static CardRegistry inst;
    return inst;
  }

  template<class C>
  int registerCard() {
    std::cout << "hello" << std::endl;
    auto name = C().getName();
    factories[name] = []() { return new C(); };
    return factories.size() - 1;
  }

  template<class C>
  std::unique_ptr<C> create(std::string const& name) {
    Card* card = factories.at(name)();
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
  std::unordered_map<std::string, Ctor> factories;
};
