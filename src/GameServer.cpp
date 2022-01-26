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

std::string creatureToString(Creature* c){
    std::stringstream sstm;
    sstm << c->getName() << " " << c->getPower() << "/" << c->getToughness();
    
    return sstm.str();
}

GameServer::GameServer() {

    std::cout << "Creating server..." << std::endl;

    this->sendBuffer = {"void", "void"};
    this->choice = {-1, -1};
    this->multiChoices = {std::vector<int>(),std::vector<int>()};
}

GameServer::~GameServer(){}

void GameServer::send(int player, std::string s){
    this->sendBuffer[player] = s;
    // std::cout << s << std::endl;
    this->listen();
}

int GameServer::getChoice(int player) {
    
    this->choice[player] = -2; // We want to receive from this player
        
    while(this->choice[player] == -2)
        this->listen();

    return this->choice[player];

}

std::vector<int> GameServer::getMultiChoices(int player) {

    this->choice[player] = -2; // We want to receive from this player

    while(this->choice[player] == -2)
        this->listen();
    
    return this->multiChoices[player];

}


void GameServer::listen(){
    std::cout << "Listening..." << std::endl;
    svr.listen("0.0.0.0", 8080);
}

void GameServer::init(){
    svr.set_logger([](const auto& req, const auto& res) {
        std::cout << req.body.data() << std::endl;
        std::cout << res.body.data() << std::endl;
    });

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

        // std::cout << this->sendBuffer[playerI] << std::endl;

        res.set_content(this->sendBuffer[playerI], "application/json");
        this->sendBuffer[playerI] = "void";
        svr.stop();
    });

    svr.Get(R"(/choice/(\d+)/(-?\d+))", [&](const Request &req, Response &res) {
        
        int player = std::stoi(req.matches[1]);

        if(this->choice[player] == -2) { // We have a choice to receive for this player
            int choice = std::stoi(req.matches[2]);

            this->choice[player] = choice;

            res.set_content("Player "+ std::to_string(player) + " selected " + std::to_string(choice), "text/plain");
            svr.stop();
        }

    });

    svr.Get(R"(/multichoices/(\d+)/(\d+(,\d+)*))", [&](const Request &req, Response &res) {
        
        int player = std::stoi(req.matches[1]);

        if(this->choice[player] == -2) { // We have choices to receive for this player
            
            this->multiChoices[player] = {};
            this->choice[player] = -1;

            std::string s = req.matches[2];

            std::regex word_regex("(\\d+)");
            auto words_begin = 
                std::sregex_iterator(s.begin(), s.end(), word_regex);
            auto words_end = std::sregex_iterator();
                        
            for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
                std::smatch match = *i;
                std::string match_str = match.str();
                int choice = std::stoi(match_str);

                this->multiChoices[player].push_back(choice);

            }
            
            res.set_content("Player "+ std::to_string(player) + " selected " + std::to_string(this->multiChoices[player].size()) + " options", "text/plain");
            svr.stop();
        }

    });


    // svr.Post("/duel", [&](const Request& req, Response& res) {
        
    //     auto result = req.get_param_value("blockers");

    //     res.set_content(result, "text/plain");
    // });

    
    
}
