#pragma once

#include "Mana.hpp"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

class Ability;

/** A card. (abstract class) */
class Card {

public:
  enum class State {
    LIBRARY, HAND, GRAVEYARD, BATTLEFIELD
  };

  Card();
  virtual ~Card();
  Card(Card const&) = delete;
  Card& operator=(Card const&) = delete;

  State getState() const;
  void setState(State state);
  void tap();
  void untap();
  bool isTapped() const;
  bool hasAbility(Ability const* ability) const;

  virtual std::string getName() const = 0;
  virtual std::string getType() const = 0;
  virtual std::string getDesc() const { return "No description available."; }
  virtual ManaCost getCost() const = 0;
  virtual Mana getMana() const = 0;
  virtual std::vector<Ability const*> getAbilities() const { return {}; }
  virtual std::string getId() const {return this->getName(); }

  virtual std::string toJson() const {

    using namespace rapidjson;

    StringBuffer s;
    Writer<StringBuffer> writer(s);

    writer.StartObject();
    writer.Key("dataType");
    writer.String("card");
    writer.Key("id");
    writer.String(this->getId().c_str());
    writer.Key("isTapped");
    writer.Bool(this->isTapped());
    writer.Key("state");
    writer.Int(static_cast<int>(this->getState()));
    writer.EndObject();

    return s.GetString();

  }

  virtual void fromJson(std::string json) {
    using namespace rapidjson;

    Document d;
    d.Parse(json.c_str());

    d["isTapped"].GetBool() ? this->tap() : this->untap() ;
    this->setState(static_cast<State>(d["state"].GetInt()));

  }

private:
  State state;
  bool tapped;
};
