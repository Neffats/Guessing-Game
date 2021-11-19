#pragma once

#include <mutex>
#include <map>

#include "common.hpp"

using namespace std;

// Singleton class.
class ScoreBoard {
private:
  //static ScoreBoard instance;

  std::map<std::string, Score> _scores;

  std::mutex mux;

  ScoreBoard() {};

public:
  static ScoreBoard& GetInstance();
  ScoreBoard(ScoreBoard &other) = delete;

  void RecordCorrect(std::string player);
  void RecordIncorrect(std::string player);
  Score GetScore(std::string player);
};
