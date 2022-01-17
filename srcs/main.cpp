#include "Server/Server.hpp"

// TODO:: parse args
/**
 * Parse args, if they okay execute server
 * @return
 */
int main(int argc, char** argv) {
  Server* app;
  if (argc == 3)
    app = new Server(inet_addr("127.0.0.1"), atoi(argv[1]), argv[2]);
  else
    app = new Server(inet_addr("127.0.0.1"), 6667, "1");

//  Server* app = Server();
//  Server *app = Server::getInstance();
  app->start();
  return (0);
}
