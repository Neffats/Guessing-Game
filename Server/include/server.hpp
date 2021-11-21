#pragma once

#include <iostream>
#include <arpa/inet.h>
#include <mutex>
#include <vector>

#include "network.hpp"
#include "common.hpp"
#include "controller.hpp"

class IServer {
public:
  IServer(){};
  virtual ~IServer(){};
  virtual void Run()=0;
  virtual void Init()=0;
};

class TcpServer: public IServer {
private:
  INetwork* _net;
  IController* _app;

  std::vector<std::thread> _handlers;
  
  // Keep track of how many available slots.
  int _current_players;
  std::mutex _current_players_mux;

  void HandleConnection(ISession* s);
  void HandleGuess(ISession* s, std::string player, std::string guess);
  void HandleGetScore(ISession* s, std::string player);
  
public:
  TcpServer(IController* app, INetwork* net, int players);
  ~TcpServer(){};
  void Run();
  void Init(); 
};
