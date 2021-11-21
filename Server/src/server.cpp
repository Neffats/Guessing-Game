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

TcpServer::TcpServer(IController* app, INetwork* net, int players) {
  _current_players = players;
  _app = app;
  _net = net;
};

void TcpServer::Init() {};

void TcpServer::Run() {
  while(1) {
    ISession* s = _net->Accept();

    _current_players_mux.lock();
    if (_current_players == 0) {
      _current_players_mux.unlock();
      s->Error();
      delete s;
      
      continue;
    } 

    _current_players -= 1;
    _current_players_mux.unlock();

    std::thread t1([this, s]() {
      HandleConnection(s);
    });

    t1.detach();
    _handlers.push_back(std::move(t1));
  }
};

#define BUFFER_SIZE 512

void TcpServer::HandleConnection(ISession* s) {
  Message msg;
  
  try {
    msg = s->Read();
  } catch (std::exception& e) {
    s->Error();
    _current_players_mux.lock();
    _current_players += 1;
    _current_players_mux.unlock();
    delete s;
    return;
  }

  if (msg.type != MessageType::Connect) {
    s->Error();
    delete s;

    _current_players_mux.lock();
    _current_players += 1;
    _current_players_mux.unlock();
    
    return;
  }

  std::string player = msg.Parameter;

  s->Ok();

  std::cout << "Connected: " << player << std::endl;

  while (true) {
    try {
      msg = s->Read();
    } catch (...) {
      s->Error();
      delete s;

      _current_players_mux.lock();
      _current_players += 1;
      _current_players_mux.unlock();
      
      return;
    }

    switch (msg.type) {
    case MessageType::Connect:
      s->Error();
      break;
    case MessageType::Guess:
      HandleGuess(s, player, msg.Parameter);
      break;
    case MessageType::Get_Score:
      HandleGetScore(s, msg.Parameter);
      break;
    case MessageType::Disconnect:
      s->Ok();
      _current_players_mux.lock();
      _current_players += 1;
      _current_players_mux.unlock();
      delete s;
      return;
    case MessageType::Invalid:
      s->Error();
      break;
    }
  }
};

void TcpServer::HandleGuess(ISession* s, std::string player, std::string guess) {
  int guess_int;

  guess_int = std::stoi(guess);

  Result r = _app->CheckGuess(guess_int, player);

  s->Answer(r);

  return;
};

void TcpServer::HandleGetScore(ISession* s, std::string player) {
  Score score = _app->GetScore(player);

  s->Score(score);

  return;
};
