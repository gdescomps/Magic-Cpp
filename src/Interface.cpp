#include "Interface.hpp"
#include <iostream>

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "GameServer.hpp"

using namespace rapidjson;
using namespace std;

void Interface::tell(int player, std::string const &msg)
{
  StringBuffer s;
  Writer<StringBuffer> writer(s);

  writer.StartObject();
  writer.Key("dataType");
  writer.String("tell");
  writer.Key("msg");
  writer.String(msg.c_str());
  writer.EndObject();

  this->server->send(player, s.GetString());
}

int Interface::showMenu(int player, std::string const &msg, std::vector<MenuEntry> choices)
{

  StringBuffer s;
  Writer<StringBuffer> writer(s);

  writer.StartObject();
  writer.Key("dataType");
  writer.String("menu");
  writer.Key("msg");
  writer.String(msg.c_str());
  writer.Key("entries");
  writer.StartArray();

  for (auto e : choices)
  {
    writer.StartObject();
    writer.Key("state");
    writer.Int(static_cast<int>(e.state));
    writer.Key("text");
    writer.String(e.text.c_str());
    writer.EndObject();
  }

  writer.EndArray();
  writer.EndObject();

  this->server->send(player, s.GetString());

  return this->server->getChoice(player);
}

bool Interface::promptYesNo(int player, std::string const &msg)
{
  StringBuffer s;
  Writer<StringBuffer> writer(s);

  writer.StartObject();
  writer.Key("dataType");
  writer.String("promptYesNo");
  writer.Key("msg");
  writer.String(msg.c_str());
  writer.EndObject();

  this->server->send(player, s.GetString());

  return this->server->getChoice(player) == 1;
}

void Interface::hideAll(int player){
  StringBuffer s;
  Writer<StringBuffer> writer(s);

  writer.StartObject();
  writer.Key("dataType");
  writer.String("hideAll");
  writer.EndObject();

  this->server->send(player, s.GetString());
}
