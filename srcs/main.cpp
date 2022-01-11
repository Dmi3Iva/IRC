#include "Server.hpp"
#include <iostream>

int main(void) {
  Server *app = Server::getInstance();
  app->start();
  return (0);
}
