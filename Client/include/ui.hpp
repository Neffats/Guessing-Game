#pragma once

#include <string>

#include "controller.hpp"

class IUI {
public:
  IUI(){};
  virtual ~IUI(){};
  virtual void Run() = 0;
};

enum class CommandType {
  Connect,
  Disconnect,
  Guess,
  Get_Score,
  Invalid
};

struct Command {
    CommandType type;
    std::string parameter;
};

class CommandLine : public IUI {
private:
  IController* _app;

  Command ParseCommand(std::string cmd);
  CommandType ParseCommandType(std::string cmd);
  std::string GetCommandParameter(std::string cmd);

  void HandleGuess(Command cmd);
  void HandleGetScore(Command cmd);

  // void PrintScore(Score s);
  // void PrintResult(Result r);
  void PrintError(std::string err);
  void PrintPrompt();
public:
  CommandLine(IController* c);
  
  void Run();

};