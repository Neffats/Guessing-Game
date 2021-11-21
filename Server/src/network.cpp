#include <exception>
#include <sstream>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <mutex>
#include <thread>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "network.hpp"

TcpNetwork::TcpNetwork(int port) {
  _port = port;
};

TcpNetwork::~TcpNetwork() {
  close(_sockfd);
};  

// Creates and configures the socket.
// Throws runtime_error.
void TcpNetwork::Init() {
  struct sockaddr_in serv_addr;
  
  _sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (_sockfd < 0) {
    throw std::runtime_error("Failed to create socket");
  }

  int enable = 1;
  if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
      throw std::runtime_error("setsockopt(SO_REUSEADDR) failed");

  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(_port);

  int res = ::bind(_sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  if (res < 0) {
    throw std::runtime_error("Failed to bind to socket");
  }

  if (listen(_sockfd, 10) == -1) {
    throw std::runtime_error("Failed to listen to socket");
  }
};

// Waits for a client to connect and creates a session for that client.
// All communication to that client is then done via the session object.
ISession* TcpNetwork::Accept() {
  int new_fd;
  struct sockaddr_in client_addr;
  socklen_t client_len;

  client_len = sizeof(client_addr);
  
  new_fd = accept(_sockfd, (struct sockaddr *)&client_addr, &client_len);
  
  if (new_fd == -1) {
    throw std::runtime_error("Failed to accept new connection");
  }

  TcpSession* s = new TcpSession(new_fd, client_addr, client_len);
  return s;
};

TcpSession::TcpSession(int fd, struct sockaddr_in client_addr, socklen_t client_len) {
  _fd = fd;
  _client_addr = client_addr;
  _client_len = client_len;
};

TcpSession::~TcpSession() {
  close(_fd);
}

#define BUFFER_SIZE 512

// Read from the socket for the next message.
Message TcpSession::Read() {
  char buffer[BUFFER_SIZE];
  
  bzero(buffer, BUFFER_SIZE);
  int res = read(_fd, buffer, BUFFER_SIZE);
  if (res < 0) {
    throw std::runtime_error("Failed to read from file descriptor");
  }

  std::string raw_msg(buffer);
  Message msg;
  
  try {
    msg = ParseMessage(raw_msg);
  }
  catch (exception& e) {
    throw;
  }

  return msg;
};

Message TcpSession::ParseMessage(std::string msg) {
  MessageType type = GetMessageType(msg);

  std::string param;

  switch (type) {
  case MessageType::Disconnect:
      param = "";
      break;
  default:
      param = GetMessageParameter(msg);
  }

  Message m = { type, param };
  
  return m;
};

MessageType TcpSession::GetMessageType(std::string msg) {
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
  } else {
    return MessageType::Invalid;
  }
};

std::string TcpSession::GetMessageParameter(std::string msg) {
  std::string delim = " ";

  int delim_idx = msg.find_first_of(delim);
  
  std::string parameter = msg.substr(delim_idx+1);

  return parameter;
};

void TcpSession::SendMessage(std::string msg) {
  if ( send(_fd, msg.data(), msg.length(), 0) == -1 ) {
    throw std::runtime_error("Failed to send message");
  }
};

void TcpSession::Ok() {
  try {
    SendMessage("OK");
  } catch (...) {
    throw;
  }
  
  return;
};

void TcpSession::Error() {
  try {
    SendMessage("ERROR");
  } catch (...) {
    throw;
  }
  
  return;
};

void TcpSession::Busy() {
  try {
    SendMessage("BUSY");
  } catch (...) {
    throw;
  }
  
};

void TcpSession::Answer(Result r) {
  std::string answer;
  std::string msg;

  switch (r) {
  case Result::Correct:
    answer = "Correct";
    break;
  case Result::Higher:
    answer = "Higher";
    break;
  case Result::Lower:
    answer = "Lower";
    break;
  }

  msg = "ANSWER " + answer;

  try {
    SendMessage(msg);
  } catch (...) {
    throw;
  }

  return;
};

void TcpSession::Score(struct Score s) {
  std::stringstream msg;

  msg << "SCORE " << s.score << "/" << s.attempts;

  try {
    SendMessage(msg.str());
  } catch (...) {
    throw;
  }

  return;
}
