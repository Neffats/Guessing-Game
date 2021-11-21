#include <iostream>
#include <csignal>

//#include "controller.hpp"
#include "network.hpp"
#include "ui.hpp"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "Usage: ./GuessingGameClient [player name] [server ip address]";
    return -1;
  }

  std::string player(argv[1]);
  std::string ip(argv[2]);

  TcpNetwork* net = new TcpNetwork(ip, 5000);
  AppController* app = new AppController(player, net);
  CommandLine* ui = new CommandLine(player, app);

  app->Connect();
  ui->Run();

  return 0;
}
