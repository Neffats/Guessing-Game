#include <iostream>

#include "controller.hpp"
#include "scoreboard.hpp"

using namespace std;

AppController::AppController(UI* ui) {
  _ui = ui;
};

Result AppController::CheckGuess(int guess, std::string player) {

  std::cout << "AppController: Checking guess: " << guess << " for " << player << std::endl;
  Result r = _ui->CheckGuess(guess, player);

  std::cout << "Returned from check guess ui" << std::endl;

  ScoreBoard& sb = ScoreBoard::GetInstance();
  
  std::cout << "Accessed singleton" << std::endl;
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
