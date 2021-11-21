#include <iostream>

//#include "controller.hpp"
#include "network.hpp"
#include "ui.hpp"

int main(int argc, char* argv[]) {
  TcpNetwork* net = new TcpNetwork("127.0.0.1", 5000);
  AppController* app = new AppController("steffan", net);
  CommandLine* ui = new CommandLine(app);

  app->Connect();
  ui->Run();

  return 0;
}
