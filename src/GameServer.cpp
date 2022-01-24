#include "httplib.hpp"
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

#include "GameServer.hpp"

#include "creatures/AirBender.hpp"
#include "creatures/BrandonSoupSirup.hpp"
#include "creatures/CrownTheVirulent.hpp"
#include "creatures/JackSeparou.hpp"
#include "creatures/LeojTheMerciful.hpp"
#include "creatures/PolyNinja.hpp"
#include "creatures/RatMan.hpp"
#include "creatures/SerraAngel.hpp"
#include "creatures/Theresa.hpp"

using namespace httplib;

std::vector<std::unique_ptr<Card>> makePlayer1Deck() {
  std::vector<std::unique_ptr<Card>> res;
  res.push_back(std::make_unique<AirBender>());
  res.push_back(std::make_unique<BrandonSoupSirup>());
  res.push_back(std::make_unique<CrownTheVirulent>());
  res.push_back(std::make_unique<JackSeparou>());
  res.push_back(std::make_unique<LeojTheMerciful>());
  res.push_back(std::make_unique<PolyNinja>());
  res.push_back(std::make_unique<RatMan>());
  res.push_back(std::make_unique<SerraAngel>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Land>(Mana::BLACK));
  res.push_back(std::make_unique<Land>(Mana::BLACK));
  res.push_back(std::make_unique<Land>(Mana::BLACK));
  res.push_back(std::make_unique<Land>(Mana::BLUE));
  res.push_back(std::make_unique<Land>(Mana::BLUE));
  res.push_back(std::make_unique<Land>(Mana::BLUE));
  res.push_back(std::make_unique<Land>(Mana::GREEN));
  res.push_back(std::make_unique<Land>(Mana::GREEN));
  res.push_back(std::make_unique<Land>(Mana::GREEN));
  res.push_back(std::make_unique<Land>(Mana::RED));
  res.push_back(std::make_unique<Land>(Mana::RED));
  res.push_back(std::make_unique<Land>(Mana::RED));
  res.push_back(std::make_unique<Land>(Mana::WHITE));
  res.push_back(std::make_unique<Land>(Mana::WHITE));
  res.push_back(std::make_unique<Land>(Mana::WHITE));
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Theresa>());
  res.push_back(std::make_unique<Theresa>());
  return res;
}

GameServer::GameServer() {
    this->start();
    this->deckP1 = makePlayer1Deck();
}

GameServer::~GameServer(){}

void GameServer::start(){
    Server svr;

    svr.Get("/hi", [](const Request &req, Response &res) {
        std::cout << req.body.data() << std::endl;
        std::cout << "hello" << std::endl;
        res.set_content("Hello World!", "text/plain");
    });

    svr.Get(R"(/blockers/(\d+(,\d+)*))", [&](const Request& req, Response& res) {
        std::cout << "blockers" << std::endl;

        std::string s = req.matches[1];

        std::regex word_regex("(\\d+)");
        auto words_begin = 
            std::sregex_iterator(s.begin(), s.end(), word_regex);
        auto words_end = std::sregex_iterator();
        
        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            std::smatch match = *i;
            std::string match_str = match.str();
            std::cout << std::stoi(match_str) << '\n';
        }
        
        res.set_content("Received blockers", "text/plain");
    });

    // svr.Post("/duel", [&](const Request& req, Response& res) {
        
    //     auto result = req.get_param_value("blockers");

    //     res.set_content(result, "text/plain");
    // });

    svr.listen("0.0.0.0", 8080);
}