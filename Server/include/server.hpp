#pragma once

#include <iostream>
#include <arpa/inet.h>
#include <mutex>
#include <vector>

#include "network.hpp"
#include "common.hpp"
#include "controller.hpp"

class Server {
public:
  Server(){};
  virtual ~Server(){};
  virtual void Run()=0;
  virtual void Init()=0;
};



class TcpServer: public Server {
private:
  Network* _net;
  Controller* _app;

  std::vector<std::thread> handlers;
  
  int current_players;
  std::mutex* current_players_mux;

  void HandleConnection(Session* s);
  void HandleGuess(Session* s, std::string player, std::string guess);
  void HandleGetScore(Session* s, std::string player);
  
public:
  TcpServer(Controller* app, Network* net, int players);
  ~TcpServer(){
    delete current_players_mux;
  };
  void Run();
  void Init(); 
};
