#pragma once

#include <mutex>
#include <queue>
#include <string>
#include <thread>

#include "common.hpp"

class UI {
public:
  UI(){};
  virtual ~UI(){};
  virtual Result CheckGuess(int guess, std::string player)=0;
};

class Guess {
public:
  int guess;
  std::string player;

  Guess() {
    guess = 0;
    player = "";
  };

  Guess(int g, std::string p) {
    guess = g;
    player = p;
  };

  ~Guess(){};
};

class GuessResult {
public:
  Result res;
  std::string player;

  GuessResult(Result r, std::string p) {
    res = r;
    player = p;
  };
  ~GuessResult(){};
};

class CommandLine: public UI {
public:
  CommandLine();
  ~CommandLine(){
    //delete _guess_queue;
    //delete _guess_mux;
    //delete _result_queue;
    //delete _result_mux;
  };
  Result CheckGuess(int guess, std::string player);
  void Run();

private:
  std::thread _main_thread;
  
  std::queue<Guess*> _guess_queue;
  std::mutex _guess_mux;
  
  std::queue<GuessResult*> _result_queue;
  std::mutex _result_mux;
  
  void PrintWelcome();
  Guess* FetchGuess();
  void PutResult(GuessResult* r);
  void StartThread();
};

