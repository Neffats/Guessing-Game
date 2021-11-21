#pragma once

#include <string>
#include "common.hpp"
#include "ui.hpp"

using namespace std;

/*
The Controller is the component that groups all of the other components together. 
*/
class IController {
public:
  IController() {};
  virtual ~IController() {};
  virtual Result CheckGuess(int guess, std::string player)=0;
  virtual Score GetScore(std::string player)=0;
};

class AppController : public IController {
private:
  IUI* _ui;
public:
  AppController(IUI* ui);
  ~AppController() {};
  Result CheckGuess(int guess, std::string player);
  Score GetScore(std::string player);
};
