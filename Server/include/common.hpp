#pragma once

#include <string>

using namespace std;

enum class Result {
  Correct,
  Higher,
  Lower
};

struct Score {
  int score, attempts;
};

enum class MessageType {
  Connect,
  Disconnect,
  Guess,
  Get_Score,
  Invalid
};

struct Message {
  MessageType type;
  std::string Parameter;
};
