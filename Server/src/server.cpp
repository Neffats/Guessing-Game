#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <mutex>
#include <thread>

#include "server.hpp"

TcpServer::TcpServer(char* ip, int port, int players) {
  _ip = ip;
  _port = port;
  current_players = players;

  current_players_mux = new std::mutex();
};
 
void TcpServer::Init() {
  struct sockaddr_in serv_addr;
  
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd < 0) {
    perror("ERROR opening socket");
  }

  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(_port);

  if (bind(socketfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("ERROR on binding");
  }

  if (listen(socketfd, 10) == -1) {
    perror("ERROR on listen");
  }
  
};

void TcpServer::Run() {
  int new_fd;
  struct sockaddr_in client_addr;
  socklen_t client_len;

  client_len = sizeof(client_addr);

  while(1) {
    new_fd = accept(socketfd, (struct sockaddr *)&client_addr, &client_len);
    if (new_fd == -1) {
      perror("Error accepting connections!");
      continue;
    }

    current_players_mux->lock();
    if (current_players == 0) {
      send(new_fd, "busy", 5, 0);
      close(new_fd);
      current_players_mux->unlock();
      continue;
    } 

    current_players -= 1;
    current_players_mux->unlock();

    std::thread t1([this, new_fd]() {
      HandleConnection(new_fd);
    });

    
  }
};

#define BUFFER_SIZE 256

void TcpServer::HandleConnection(int fd) {
  char buffer[BUFFER_SIZE];
  
  bzero(buffer, BUFFER_SIZE);
  int res = read(fd, buffer, BUFFER_SIZE);
  if (res < 0) {
    perror("HandleConnection: failed to read from socket");
   
  }

  std::cout << buffer << std::endl;

  std::string guess_str(buffer);
  int guess_int;
  
  try {
    guess_int = std::stoi(guess_str);
  }
  catch (const std::exception& e) {

    std::cout << "HandleConnection: Exception caught \"" << e.what() << "\"\n";
    
    if (send(fd, "Error", 5, 0) == -1) {
      std::cout << "Failed to send error message: " << std::endl;
    }
    
    close(fd);

    current_players_mux->lock();
    current_players += 1;
    current_players_mux->unlock();
    
    return;
  }

  Result guess_result = app->CheckGuess(guess_int);

  std::string message;
  int msg_len;

  switch (guess_result) {
  case Result::Correct:
    message = "Correct";
    msg_len = 8;
    break;
  case Result::Higher:
    message = "Higher";
    msg_len = 7;
    break;
  case Result::Lower:
    message = "Lower";
    msg_len = 6;
    break;
  }
  
  if (send(fd, message.c_str(), msg_len, 0) == -1) {
    perror("HandleConnection: failed to write to socket");
  }

  current_players_mux->lock();
  current_players += 1;
  current_players_mux->unlock();

  close(fd);

  return;
  
};
