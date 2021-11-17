#include <iostream>
#include "server.hpp"

int main(int argc, char* argv[]) {
  TcpServer serv = TcpServer("127.0.0.1", 5000, 2);

  serv.Init();
  serv.Run();
}
