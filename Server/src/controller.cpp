#include <iostream>

#include "controller.hpp"
#include "scoreboard.hpp"

using namespace std;

AppController::AppController(IUI* ui) {
  _ui = ui;
};

Result AppController::CheckGuess(int guess, std::string player) {
  Result r = _ui->CheckGuess(guess, player);


  ScoreBoard& sb = ScoreBoard::GetInstance();
  
  switch (r) {
  case Result::Correct:
    sb.RecordCorrect(player);
    break;
  default:
    sb.RecordIncorrect(player);
    break;
  }

  return r;
};

Score AppController::GetScore(std::string player) {
  ScoreBoard& sb = ScoreBoard::GetInstance();
  Score s = sb.GetScore(player);
  return s;
};
