#include <iostream>
#include <mutex>

#include "scoreboard.hpp"

using namespace std;

ScoreBoard& ScoreBoard::GetInstance() {
    static ScoreBoard sb;
    return sb;
}

void ScoreBoard::RecordCorrect(std::string player) {
  const std::lock_guard<std::mutex> lock(mux);
  
  if (_scores.count(player) == 0) {
    Score new_score = { 1, 1 };
    _scores[player] = new_score;
  } else {
    _scores[player].score += 1;
    _scores[player].attempts += 1;
  }
}

void ScoreBoard::RecordIncorrect(std::string player) {
  const std::lock_guard<std::mutex> lock(mux);

  if (_scores.count(player) == 0) {
    Score new_score = { 0, 1 };
    _scores[player] = new_score;
  } else {
    _scores[player].attempts += 1;
  }
};

Score ScoreBoard::GetScore(std::string player) {
  const std::lock_guard<std::mutex> lock(mux);

  if (_scores.count(player) == 0) {
    Score s = {0, 0};
    return s;
  } else {
    return _scores[player];
  }
}
