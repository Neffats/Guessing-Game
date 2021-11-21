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

  std::mutex _mux;

  ScoreBoard() {};

public:
  static ScoreBoard& GetInstance();

  // Not cloneable
  ScoreBoard(ScoreBoard &other) = delete;
  // Not copyable
  void operator=(const ScoreBoard &) = delete;

  void RecordCorrect(std::string player);
  void RecordIncorrect(std::string player);
  Score GetScore(std::string player);
};
