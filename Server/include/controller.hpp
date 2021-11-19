#pragma once

#include <string>
#include "common.hpp"
#include "ui.hpp"

using namespace std;

class Controller {
public:
  Controller() {};
  virtual ~Controller() {};
  virtual Result CheckGuess(int guess, std::string player)=0;
  virtual Score GetScore(std::string player)=0;
};

class AppController : public Controller {
private:
  UI* _ui;
public:
  AppController(UI* ui);
  ~AppController() {};
  Result CheckGuess(int guess, std::string player);
  Score GetScore(std::string player);
};
