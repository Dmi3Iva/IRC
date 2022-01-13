#include "NickCommand.hpp"

NickCommand::NickCommand(Context *ctx) : ACommand(name, ctx), _name("NICK") {}

void NickCommand::execute(User *user, string cmd) {
  cout << "Execute command" << cmd << endl;
  user->setNickname(cmd);
  if (user->getIsRegistered()) {
    cout << "TODO:: notify all user channels participants about changing the nickname" << endl;
  }
};
