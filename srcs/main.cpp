#include "Application.hpp"
#include <csignal>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;
using std::string;

void signalHandler(int signum) {
  cout << "Interrupt signal (" << signum << ") received.\n";

  Application *app = Application::getInstance();
  app->stop();

  exit(signum);
}

int main(void) {
  signal(SIGINT, signalHandler);
  Application *app = Application::getInstance();
  app->start();
  return (0);
}
