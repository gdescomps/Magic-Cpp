#pragma once

#include <string>

#include "Card.hpp"

#include "Duel.hpp"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


class Creature : public Card {

private:
    int power, basePower;
    int toughness, baseToughness;

public:
    Creature(int power, int toughness) 
        : power(power), basePower(power), toughness(toughness), baseToughness(toughness) {}

    virtual std::string getName() const override = 0;
    virtual std::string getType() const override { return "Creature"; }

    int getBasePower() const { return this->basePower; }
    int getPower() const { return this->power; }
    void setPower(int p) { this->power = p; }

    int getBaseToughness() const { return this->baseToughness; }
    int getToughness() const { return this->toughness; }
    void setToughness(int t) { this->toughness = t; }

    virtual void attack(Creature* adversary);
    
    virtual std::string toJson() const override { 
        using namespace rapidjson;

        Document d;
        d.Parse(Card::toJson().c_str());

        d.AddMember("power", this->getPower(), d.GetAllocator());
        d.AddMember("basePower", this->getBasePower(), d.GetAllocator());
        d.AddMember("toughness", this->getToughness(), d.GetAllocator());
        d.AddMember("baseToughness", this->getBaseToughness(), d.GetAllocator());

        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        d.Accept(writer);

        return buffer.GetString(); 
    
    }

};
