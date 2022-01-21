#include "UserCommand.hpp"

UserCommand::UserCommand(string serverName, userVector *usersPtr, channelMap *channelsPtr)
    : ACommand(serverName, usersPtr, channelsPtr) {
  _name = "USER";
  _description = "USER <username> <hostname> <servername> <realname>";
}

/**
 * Command: USER
 * Parameters: <username> <hostname> <servername> <realname>
 * example: "USER Dmitrii * localhost :Dmitrii Ivanov"
 * @param user
 * @param cmd
 */
void UserCommand::execute(User *user, string cmd) {
  cout << "Execute command" << cmd << endl;
  user->setRealname(cmd.substr(cmd.find(':') + 1));
  vector<string> userInfo = ft_split(cmd, " ");
  user->setUsername(userInfo[0]);

  if (!user->getNickname().empty() && !user->getRealname().empty() && !user->getUsername().empty()) {
    user->setIsRegistered(true);
    string msg = RPL_MOTDSTART(_serverName, user->getNickname()) + //
                 RPL_MOTD(_serverName, user->getNickname()) +      //
                 RPL_ENDOFMOTD(_serverName, user->getNickname());  //
    sendMessage(user->getFD(), msg);
  }
}