#include "ui.hpp"
#include <thread>
#include <chrono>
#include <iostream>
//#include <sstream>

CommandLine::CommandLine() {
  //_guess_queue = new std::queue<Guess*>;
  //_guess_mux = new std::mutex;

  //_result_queue = new std::queue<GuessResult*>;
  //_result_mux = new std::mutex;
};

Result CommandLine::CheckGuess(int guess, std::string player) {
  Guess* g = new Guess(guess, player);
  
  _guess_mux.lock();
  _guess_queue.emplace(g);
  _guess_mux.unlock();

  std::cout << "Emplaced guess into ui queue" << std::endl;

  while (true) {
    _result_mux.lock();
    if (_result_queue.empty()) {
      _result_mux.unlock();
      continue;
    }

    GuessResult* r = _result_queue.front();
    if (r->player == player) {
      _result_queue.pop();
      _result_mux.unlock();

      Result res = r->res;
      delete r;

      return res;

    }

    _result_mux.unlock();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
};

void CommandLine::Run() {
  std::thread t([this] () {
    StartThread();
  });

  t.detach();
  _main_thread = std::move(t);
};

void CommandLine::StartThread() {
  PrintWelcome();

  std::cout << "Waiting for incoming guesses..." << std::endl;

  while (true) {
    _guess_mux.lock();
    bool empty = _guess_queue.empty();
    _guess_mux.unlock();
    if (!empty) {
      Guess* g = FetchGuess();

      std::cout << "Received guess " << g->guess << "from player " << g->player << std::endl;
      std::cout << "Is this (C)orrect/(H)igher/(L)ower: ";

      std::string resp;

      std::cin >> resp; 

      std::string plr = g->player;
      Result res;
      
      if (resp == "C") {
        res = Result::Correct;
      } else if (resp == "H") {
        res = Result::Higher;
      } else if (resp == "L") {
        res = Result::Lower;
      } else {
        // Handle stuff
      }

      GuessResult* r = new GuessResult(res, plr);
      PutResult(r);
      delete g;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
};

Guess* CommandLine::FetchGuess() {
  _guess_mux.lock();
  Guess* g = _guess_queue.front();
  _guess_queue.pop();
  _guess_mux.unlock();

  return g;
};

void CommandLine::PutResult(GuessResult* r) {
  _result_mux.lock();
  _result_queue.push(r);
  _result_mux.unlock();
};

void CommandLine::PrintWelcome() {
  std::cout << "Welcome to the guessing server!" << std::endl;
};
