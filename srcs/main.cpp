#include "Server.hpp"

int main(void) {
  Server *app = Server::getInstance();
  app->start();
  return (0);
}
