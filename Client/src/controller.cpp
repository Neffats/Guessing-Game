#include "controller.hpp"

AppController::AppController(std::string name, INetwork* n) {
    _player = name;
    _net = n;
};

Result AppController::SendGuess(int guess) {
    Result r;
    try {
        r = _net->SendGuess(guess);
    } catch (...) {
        throw;
    }

    return r;
};

Score AppController::GetScore(std::string player) {
    Score s;
    try {
        s  = _net->GetScore(player);
    } catch (...) {
        throw;
    }

    return s;
};

void AppController::Connect() {
    _net->Connect(_player);
}

void AppController::Disconnect() {
    _net->Disconnect(_player);
};