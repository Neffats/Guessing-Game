#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "common.hpp"

class ISession {
public:
  ISession() {};
  virtual ~ISession() {};
  virtual Message Read()=0;
  virtual void Ok()=0;
  virtual void Error()=0;
  virtual void Busy()=0;
  virtual void Answer(Result r)=0;
  virtual void Score(struct Score s)=0;
};

class INetwork {
public:
  INetwork() {};
  virtual ~INetwork(){};
  virtual void Init()=0;
  virtual ISession* Accept()=0;
};


class TcpNetwork: public INetwork {
private:
  int _port;
  int _sockfd;
  
public:
  TcpNetwork(int port);
  ~TcpNetwork();
  void Init();
  ISession* Accept();
};

class TcpSession: public ISession {
private:
  int _fd;
  struct sockaddr_in _client_addr;
  socklen_t _client_len;

  Message ParseMessage(std::string msg);
  MessageType GetMessageType(std::string msg);
  std::string GetMessageParameter(std::string msg);
  void SendMessage(std::string msg);
public:
  TcpSession(int fd, struct sockaddr_in client_addr, socklen_t client_len);
  ~TcpSession();

  Message Read();
  void Ok();
  void Error();
  void Busy();
  void Answer(Result r);
  void Score(struct Score s);
};
