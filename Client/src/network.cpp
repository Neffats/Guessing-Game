#include "network.hpp"
#include <exception>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>


#include <arpa/inet.h>

TcpNetwork::TcpNetwork(std::string ip, int port) {
    _ip = ip;
    _port = port;
};

TcpNetwork::~TcpNetwork() {
    close(_sockfd);
}

void TcpNetwork::Connect(std::string player) {
    // Taken from https://beej.us/guide/bgnet/html/#client-server-background
    int sockfd, numbytes;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET_ADDRSTRLEN];


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(_ip.c_str(), std::to_string(_port).c_str(), &hints, &servinfo)) != 0) {
        throw std::runtime_error("Failed to get address info");
    }

    // Maybe there's a more C++ way to do this, but we'll stick with C way for now.
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((_sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            
            continue;
        }

        if (connect(_sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(_sockfd);
            throw std::runtime_error("Failed to connect to socket");
        }

        break;
    }

    if (p == NULL) {
        throw std::runtime_error("Failed to connect");
    }

    freeaddrinfo(servinfo); // all done with this structure

    std::string msg = "CONNECT " + player;
    SendMessage(msg);

    Message resp = Read();
    switch (resp.type) {
    case MessageType::Ok:
        return;
    case MessageType::Error:
        throw std::runtime_error("Something went wrong on the server side");
    case MessageType::Busy:
        throw std::runtime_error("Server is busy");
    default:
        throw std::runtime_error("Received unexpected response from server");
    }

}

void TcpNetwork::Disconnect(std::string player) {
    std::string msg = "DISCONNECT " + player;
    try {
        SendMessage(msg);
    } catch (std::exception& e) {
        throw std::runtime_error("Failed to disconnect: " + std::string(e.what()));
    }

    Message resp = Read();


    close(_sockfd);
}

Result TcpNetwork::SendGuess(int guess) {
    std::string msg = "GUESS " + std::to_string(guess);

    SendMessage(msg);
    Message resp = Read();

    switch (resp.type) {
    case MessageType::Error:
        throw std::runtime_error("An error occured when sending guess");
    case MessageType::Answer:
        if (resp.parameter == "Correct") {
            return Result::Correct;
        } else if (resp.parameter == "Lower") {
            return Result::Lower;
        } else if (resp.parameter == "Higher") {
            return Result::Higher;
        } else {
            throw std::runtime_error("Received unexpected answer from server: " + resp.parameter);
        }
        break;
    default:
        throw std::runtime_error("Received unexpected message from server.");
    }
}

Score TcpNetwork::GetScore(std::string player) {
    std::string msg = "GET_SCORE " + player;
    try {
        SendMessage(msg);
    } catch (std::exception& e) {
        throw std::runtime_error(std::string("Failed to GET_SCORE: ") + std::string(e.what()));
    }
    
    Message resp;
    try {
        resp = Read();
    } catch (std::exception& e) {
        throw std::runtime_error(std::string("Failed to read GET_SCORE response: ") + std::string(e.what()));
    }

    switch (resp.type) {
    case MessageType::Score:
        break;
    case MessageType::Error:
        throw std::runtime_error("Received error from server after GET_SCORE");
    default:
        throw std::runtime_error("Received unexpected response from server to GET_SCORE");
    }

    std::string delim = "/";
    int delim_idx = resp.parameter.find_first_of(delim);

    std::string correct = resp.parameter.substr(0, delim_idx);
    std::string attempts = resp.parameter.substr(delim_idx+1);

    int correct_int = std::stoi(correct);
    int attempts_int = std::stoi(attempts);

    Score s = {correct_int, attempts_int};

    return s;
}

void TcpNetwork::SendMessage(std::string msg) {
    std::string msg_send = msg + "\r\n";
    if ( send(_sockfd, msg_send.data(), msg_send.length(), 0) == -1 ) {
        throw std::runtime_error("Failed to send message");
    }
};

#define BUFFER_SIZE 512

Message TcpNetwork::Read() {
  char buffer[BUFFER_SIZE];
  
  bzero(buffer, BUFFER_SIZE);
  int res = read(_sockfd, buffer, BUFFER_SIZE);
  if (res < 0) {
    throw std::runtime_error("Failed to read from file descriptor");
  }

  std::string raw_msg(buffer);
  Message msg;
  
  try {
    msg = ParseMessage(raw_msg);
  }
  catch (std::exception& e) {
    throw;
  }

  return msg;
};

Message TcpNetwork::ParseMessage(std::string msg) {
    MessageType type = GetMessageType(msg);
    std::string param;
  
    switch (type) {
    case MessageType::Ok:
    case MessageType::Busy:
    case MessageType::Error:
        param = "";
        break;
    default:
        param = GetMessageParameter(msg);
        break;
  }

  Message m = { type, param };
  
  return m;
};

MessageType TcpNetwork::GetMessageType(std::string msg) {
  std::string delim = " ";

  int delim_idx = msg.find_first_of(delim);
  std::string msg_type = msg.substr(0, delim_idx);

  if (msg_type == "CONNECT") {
    return MessageType::Connect;
  } else if (msg_type == "DISCONNECT") {
    return MessageType::Disconnect;
  } else if (msg_type == "GUESS") {
    return MessageType::Guess;
  } else if (msg_type == "GET_SCORE") {
    return MessageType::Get_Score;
  } else if (msg_type == "OK") {
      return MessageType::Ok;
  } else if (msg_type == "ERROR") {
      return MessageType::Error;
  } else if (msg_type == "BUSY") {
      return MessageType::Busy;
  } else if (msg_type == "ANSWER") {
      return MessageType::Answer;
  } else if (msg_type == "SCORE") {
      return MessageType::Score;
} else {
    return MessageType::Invalid;
  }
};

std::string TcpNetwork::GetMessageParameter(std::string msg) {
  std::string delim = " ";

  int delim_idx = msg.find_first_of(delim);
  
  std::string parameter = msg.substr(delim_idx+1);

  return parameter;
};