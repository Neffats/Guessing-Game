#pragma once

#include "network.hpp"
#include "common.hpp"

#include <string>

class IController {
public:
  IController(){};
  virtual ~IController(){};
  virtual Result SendGuess(int guess) = 0;
  virtual Score GetScore(std::string player) = 0;
  virtual void Connect()=0;
  virtual void Disconnect()=0;
};

class AppController : public IController {
private:
    INetwork* _net;
    std::string _player;

public:
    AppController(std::string name, INetwork* n);
    ~AppController() {};
    Result SendGuess(int guess);
    Score GetScore(std::string player);
    void Connect();
    void Disconnect();
};