#include <iostream>

#include "network.hpp"
#include "server.hpp"
#include "controller.hpp"
#include "ui.hpp"



int main(int argc, char* argv[]) {
  CommandLine* ui = new CommandLine();
  AppController* ta = new AppController(ui);
  TcpNetwork* net = new TcpNetwork(5000);

  net->Init();
  
  TcpServer* srv = new TcpServer(ta, net, 2);

  ui->Run();


  srv->Init();
  srv->Run();
}
