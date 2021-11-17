#pragma once

#include <iostream>
#include <arpa/inet.h>
#include "controller.hpp"
#include <mutex>

class Server {
public:
  Server() {};
  virtual ~Server() {};
  virtual void Run()=0;
  virtual void Init()=0;
};

class TcpServer: public Server {
private:
  int socketfd;
  char* _ip;
  int _port;
  Controller* app;
  
  int current_players;
  std::mutex* current_players_mux;

  void HandleConnection(int fd);
  //std::thread SpawnHandler(int fd);
  

public:
  TcpServer(char* ip, int port, int players, Controller* app);
  void Run();
  void Init(); 
};
