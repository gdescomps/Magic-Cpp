#include "../lib/httplib.hpp"
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

#include "GameServer.hpp"
#include "Game.hpp"


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

std::string creatureToString(Creature* c){
    std::stringstream sstm;
    sstm << c->getName() << " " << c->getPower() << "/" << c->getToughness();
    
    return sstm.str();
}

GameServer::GameServer() {

    std::cout << "Creating server..." << std::endl;

    auto deck = makePlayer1Deck();

    std::vector<Card*> res(deck.size());

    for(size_t i = 0; i < deck.size(); i++) {
        res[i] = deck[i].get();
    }

    this->deckP1 = res;

    this->sendBuffer = {"void", "void"};
    this->currentPlayer = 0;

}

GameServer::~GameServer(){}

void GameServer::send(std::string s){
    this->sendBuffer[this->currentPlayer] = s;
    // std::cout << s << std::endl;
}

void GameServer::start(){
    Server svr;

    svr.Get("/hi", [](const Request &req, Response &res) {
        std::cout << req.body.data() << std::endl;
        std::cout << "hello" << std::endl;
        res.set_content("Hello World!", "text/plain");
    });

    svr.Get(R"(/attacker/(\d+))", [&](const Request& req, Response& res) {

        int attackerI = std::stoi(req.matches[1]);

        Creature* c = static_cast<Creature*>(this->deckP1[attackerI]);

        std::cout << "Attacker : " << std::endl << attackerI << " - " << creatureToString(c) << std::endl;
        

        Duel* d = new Duel();

        

        d->attacker = c;
        
        this->duelHistory.push_back(*d); 

        res.set_content("Received attacker", "text/plain");
    });

    svr.Get(R"(/blockers/(\d+(,\d+)*))", [&](const Request& req, Response& res) {
        std::cout << "Blockers : " << std::endl;

        std::string s = req.matches[1];

        std::regex word_regex("(\\d+)");
        auto words_begin = 
            std::sregex_iterator(s.begin(), s.end(), word_regex);
        auto words_end = std::sregex_iterator();
        
        Duel* d = &(this->duelHistory[this->duelHistory.size()-1]);
        
        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            std::smatch match = *i;
            std::string match_str = match.str();
            int blockerI = std::stoi(match_str);

            auto c = static_cast<Creature*>(this->deckP1[blockerI]);

            std::cout << blockerI << " - " << creatureToString(c) << std::endl;

            d->blockers.push_back(c); 

        }

        d->performDuel();

        std::string result = "Result : \n";
        
        result += "Attacker : \n" + creatureToString(d->attacker) + "\n";

        for(auto c : d->blockers){
            result += "Blocker : \n" + creatureToString(c) + "\n";
        }

        std::cout << result << std::endl;
        
        res.set_content(result, "text/plain");
    });

    svr.Get(R"(/poll/(\d+))", [&](const Request &req, Response &res) {
        
        int playerI = std::stoi(req.matches[1]);

        // std::cout << req.body.data() << std::endl;

        std::cout << this->sendBuffer[playerI] << std::endl;

        res.set_content(this->sendBuffer[playerI], "application/json");
    });

    // svr.Post("/duel", [&](const Request& req, Response& res) {
        
    //     auto result = req.get_param_value("blockers");

    //     res.set_content(result, "text/plain");
    // });

    svr.listen("0.0.0.0", 8080);
}
