#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <mutex>
#include <thread>

#include "server.hpp"

TcpServer::TcpServer(Controller* app, Network* net, int players) {
  current_players = players;
  _app = app;
  _net = net;
  

  current_players_mux = new std::mutex();
};

void TcpServer::Init() {
  
};

void TcpServer::Run() {
  while(1) {
    Session* s = _net->Accept();

    std::cout << "Connection received!" << std::endl;

    current_players_mux->lock();
    if (current_players == 0) {
      current_players_mux->unlock();
      s->Error();
      delete s;
      
      std::cout << "Server Busy" << std::endl;
      
      continue;
    } 

    current_players -= 1;
    current_players_mux->unlock();

    std::thread t1([this, s]() {
      HandleConnection(s);
    });

    t1.detach();
    std::cout << "thread created" << std::endl;
    handlers.push_back(std::move(t1));
  }
};

#define BUFFER_SIZE 512

void TcpServer::HandleConnection(Session* s) {
  Message msg = s->Read();

  if (msg.type != MessageType::Connect) {
    s->Error();
    delete s;

    current_players_mux->lock();
    current_players += 1;
    current_players_mux->unlock();
    
    return;
  }

  std::string player = msg.Parameter;

  s->Ok();

  std::cout << "Connected: " << player << std::endl;

  while (true) {
    msg = s->Read();

    switch (msg.type) {
    case MessageType::Connect:
      s->Error();
      break;
    case MessageType::Guess:
      std::cout << "Received GUESS command from: " << player << " " << msg.Parameter << std::endl;
      HandleGuess(s, player, msg.Parameter);
      break;
    case MessageType::Get_Score:
      HandleGetScore(s, msg.Parameter);
      break;
    case MessageType::Disconnect:
      s->Ok();
      current_players_mux->lock();
      current_players += 1;
      current_players_mux->unlock();
      delete s;
      return;
    case MessageType::Invalid:
      s->Error();
      break;
    }
  }
};

void TcpServer::HandleGuess(Session* s, std::string player, std::string guess) {
  int guess_int = std::stoi(guess);

  Result r = _app->CheckGuess(guess_int, player);

  s->Answer(r);

  return;
};

void TcpServer::HandleGetScore(Session* s, std::string player) {
  Score score = _app->GetScore(player);

  s->Score(score);

  return;
};
