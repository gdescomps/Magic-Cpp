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
  Land(Mana mana);
  Land();

  Mana getMana() const override;  
  std::string getName() const override;  
  std::string getType() const override;  
  ManaCost getCost() const override;
  int getCardId() const override;

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

