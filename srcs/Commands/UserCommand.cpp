#include "UserCommand.hpp"

UserCommand::UserCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr)
	: ACommand(serverName, usersPtr, channelsPtr, "USER")
{
	_description = "USER <username> <hostname> <servername> <:realname>";
}

/**
 * Command: USER
 * Parameters: <username> <hostname> <servername> <realname>
 * example: "USER Dmitrii * localhost :Dmitrii Ivanov"
 * @param user
 * @param cmd
 */
void UserCommand::execute(User* user, string cmd)
{
	if (user->getIsUserPerformed()) {
		sendMessage(user->getFD(), ERR_ALREADYREGISTRED(_serverName, user->getNickname()));
		return;
	}

	size_t colonPos = cmd.find(':');
	if (colonPos == string::npos) {
		sendMessage(user->getFD(), ERR_NEEDMOREPARAMS(_serverName, user->getNickname(), "USER"));
		return;
	}

	string realname = _constructRealnameAndEraseFromCmd(cmd, colonPos);
	if (realname.size() == 0) {
		sendMessage(user->getFD(), ERR_NEEDMOREPARAMS(_serverName, user->getNickname(), "USER"));
		return;
	}

	vector<string> userInfo = ft_split(cmd, " ");
	if (userInfo.size() != 3) {
		sendMessage(user->getFD(), ERR_NEEDMOREPARAMS(_serverName, user->getNickname(), "USER"));
		return;
	}

	user->setRealname(realname);
	user->setUsername(userInfo[0]);
	user->setHostname(userInfo[1]);
	user->setIsUserPerformed(true);

	if (!user->isRegistered() && user->getIsNickPerformed() && user->getIsUserPerformed()) {
		user->setIsRegistered(true);
		string msg = RPL_MOTDSTART(_serverName, user->getNickname())
			RPL_MOTD(_serverName, user->getNickname())
				RPL_ENDOFMOTD(_serverName, user->getNickname());
		sendMessage(user->getFD(), msg);
	}
}

string UserCommand::_constructRealnameAndEraseFromCmd(string& cmd, size_t colonPos)
{
	string name = cmd.substr(colonPos + 1);
	unsigned long whiteSpacesCount = 0;
	string whiteSpaces = " \t";
	for (int i = 0; name[i]; i++) {
		if (whiteSpaces.find(name[i]) != string::npos) {
			whiteSpacesCount++;
		}
	}
	if (whiteSpacesCount == name.size())
		return ("");
	cmd.erase(colonPos, cmd.size());
	eraseSpacesInFront(name);
	return (name);
}