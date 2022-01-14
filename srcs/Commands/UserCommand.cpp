#include "UserCommand.hpp"

UserCommand::UserCommand(vector<User> *usersPtr, vector<Channel> *channelsPtr)
    : _ACommand(userPtr, channelsPtr), _name("USER") {}

/**
 * Command: USER
 * Parameters: <username> <hostname> <servername> <realname>
 * example: "USER Dmitrii * localhost :Dmitrii Ivanov"
 * @param user
 * @param cmd
 */
// TODO:: should we ignore hostname and servername?
void UserCommand::execute(User *user, string cmd) {
  cout << "Execute command" << cmd << endl;
  user->setRealname(cmd.substr(cmd.find(':') + 1));
  vector<string> userInfo = ft_split(cmd, ' ');
  user->setUsername(userInfo[0]);

  if (!user->getNickname().empty() && !user->getRealname().empty() && !user->getUsername().empty()) {
    user->setIsRegistered(true);
  }
};