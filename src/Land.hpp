#pragma once

#include <string>
#include <array>

#include "Card.hpp"
#include "Mana.hpp"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

class Land : public Card {

public:
  Land(Mana mana) : mana(mana) {} 

  Mana getMana() const override { return mana; }
  
  std::string getName() const override { 
    static std::array<std::string, 5> const names = {
      "Plains", "Island", "Swamp", "Mountain", "Forest"
    };

    return names[(size_t)mana]; // using enum indexing order in the lookup of names
  }
  
  std::string getType() const override {
    static std::array<std::string, 5> const names = {
      "White", "Blue", "Black", "Red", "Forest"
    };
    return "Land - " + names[(size_t)mana] + " mana"; // using enum indexing order in the lookup of names
  }
  
  ManaCost getCost() const override { return ManaCost(); }

  virtual std::string toJson() const override { 
        using namespace rapidjson;

        Document d;
        d.Parse(Card::toJson().c_str());

        d.AddMember("mana", (size_t)mana, d.GetAllocator());
        
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        d.Accept(writer);

        return buffer.GetString(); 
    
    }


private:
  Mana mana;
};

