#pragma once

#include <string>

#include "common.hpp"

class INetwork {
public:
  INetwork(){};
  virtual ~INetwork(){};
  virtual void Connect(std::string player) = 0;
  virtual Result SendGuess(int guess) = 0;
  virtual Score GetScore(std::string player)=0;
  virtual void Disconnect(std::string player)=0;
};

class TcpNetwork: public INetwork {
    public:
        TcpNetwork(std::string ip, int port);
        ~TcpNetwork();
        void Connect(std::string player);
        Result SendGuess(int guess);
        Score GetScore(std::string player);
        void Disconnect(std::string player);

    private:
        int _sockfd, _port;
        std::string _ip;

        void SendMessage(std::string msg);
        Message Read();
        Message ParseMessage(std::string msg);
        MessageType GetMessageType(std::string msg);
        std::string GetMessageParameter(std::string msg);
};