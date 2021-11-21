#pragma once

enum class Result { Correct, Higher, Lower };

struct Score {
    int correct, attempts;
};

enum class MessageType {
  Connect,
  Disconnect,
  Guess,
  Get_Score,
  Answer,
  Busy,
  Ok,
  Error,
  Invalid
};

struct Message {
  MessageType type;
  std::string parameter;
};
