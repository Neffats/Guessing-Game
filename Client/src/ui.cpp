#include <iostream>
#include <exception>

#include "ui.hpp"

CommandLine::CommandLine(std::string p, IController* c) {
    _app = c;
    _player = p;
}

void CommandLine::Run() {
    while (true) {
        std::cout << "Please enter a number (type d to disconnect): ";
        std::string input;
        getline(std::cin, input);

        if (input == "d") {
            _app->Disconnect();
            return;
        }

        // try {
        //     int guess = std::stoi(input);
        // } catch (std::exception& e) {
        //     std::cout << "Error: " << e.what() << std::endl;
        //     continue;
        // }

        Command guess = { CommandType::Guess, input};

        try {
            HandleGuess(guess);
        } catch (std::exception& e) {
            std::cout << "Error sending guess: " << e.what() << std::endl;
            continue;
        }

        Score s = _app->GetScore(_player);
        std::cout << "Your current score is: " << s.correct << "/" << s.attempts << std::endl;
    }

    /*
    while (true) {
        PrintPrompt();

        std::string input;
        getline(std::cin, input);

        Command cmd;
        try {
            cmd = ParseCommand(input);
        } catch (std::exception& e) {
            PrintError(std::string("Error: ") + std::string(e.what()));
            return;
        }

        switch (cmd.type) {
        case CommandType::Guess:
            HandleGuess(cmd);
            break;
        case CommandType::Get_Score:
            HandleGetScore(cmd);
            break;
        case CommandType::Disconnect:
            _app->Disconnect();
            return;
        default:
            PrintError("Invalid command: " + cmd.parameter);
            return;
        }
    } 
    */
};

void CommandLine::HandleGuess(Command cmd) {
    int guess;

    try {
        guess = std::stoi(cmd.parameter);
    } catch (std::exception& e) {
        throw;
    }

    Result r;
    try {
        r = _app->SendGuess(guess);
    } catch (std::exception& e) {
        PrintError(e.what());
        return;
    }

    switch (r) {
    case Result::Correct:
        std::cout << "Your guess was correct!" << std::endl;
        break;
    case Result::Lower:
        std::cout << "Your guess was too low!" << std::endl;
        break;
    case Result::Higher:
        std::cout << "Your guess was too High!" << std::endl;
        break;
    }

    return;
};

void CommandLine::HandleGetScore(Command cmd) {
    Score s;
    try {
        s = _app->GetScore(cmd.parameter);
    } catch (std::exception& e) {
        PrintError(e.what());
        return;
    }

    std::cout << "The score for " << cmd.parameter << " is " << s.correct << "/" << s.attempts << std::endl;
    return;
};

void CommandLine::PrintError(std::string err) {
    std::cout << "Error: " << err << std::endl;
};

void CommandLine::PrintPrompt() {
    std::cout << "> ";
};

Command CommandLine::ParseCommand(std::string cmd) {
    CommandType type = ParseCommandType(cmd);
    std::string param;

    switch (type) {
    case CommandType::Disconnect:
        param = "";
        break;  
    default:
        param = GetCommandParameter(cmd);
    }

    Command c = { type, param};

    return c;
}

CommandType CommandLine::ParseCommandType(std::string cmd) {
    std::string delim = " ";

    int delim_idx = cmd.find_first_of(delim);
    std::string msg_type = cmd.substr(0, delim_idx);

    // TODO: Convert command to upper case.

    if (msg_type == "DISCONNECT") {
        return CommandType::Disconnect;
    } else if (msg_type == "GUESS") {
        return CommandType::Guess;
    } else if (msg_type == "GET_SCORE") {
        return CommandType::Get_Score;
    } else {
        return CommandType::Invalid;
    }

};

std::string CommandLine::GetCommandParameter(std::string cmd) {
  std::string delim = " ";

  int delim_idx = cmd.find_first_of(delim);

  std::string parameter = cmd.substr(delim_idx);
  

  return parameter;
};