#include "NickCommand.hpp"

NickCommand::NickCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr)
	: ACommand(serverName, usersPtr, channelsPtr, "NICK")
{
	_description = "NICK <nickname> [ <hopcount> ]";
}

NickCommand::~NickCommand() { }

void NickCommand::execute(User* user, string cmd)
{
	cout << "Execute command " << cmd << endl;
	eraseSpacesInFront(cmd);
	if (cmd.empty()) {
		sendMessage(user->getFD(), ERR_NONICKNAMEGIVEN(_serverName, ""));
		return;
	}
	string nick = _getNickname(cmd);
	if (!_validateNick(user, nick)) {
		return;
	}
	user->setNickname(cmd);
	user->setIsNickPerformed(true);
	if (!user->isRegistered() && user->getIsNickPerformed() && user->getIsUserPerformed()) {
		user->setIsRegistered(true);
		string msg = RPL_MOTDSTART(_serverName, user->getNickname())
			RPL_MOTD(_serverName, user->getNickname())
				RPL_ENDOFMOTD(_serverName, user->getNickname());
		sendMessage(user->getFD(), msg);
	}
}

string NickCommand::_getNickname(string cmd)
{
	int i = 0;
	string delimiters(" \n\r\0\t");
	while (delimiters.find(cmd[i]) == string::npos && cmd[i]) {
		i++;
	}
	string nick = cmd.substr(0, i);
	return (nick);
}

bool NickCommand::_validateNick(User* user, string nick)
{
	if (nick.size() >= 10 || isdigit(nick[0])) {
		sendMessage(user->getFD(), ERR_ERRONEUSNICKNAME(_serverName, "", nick));
		return (false);
	}
	string allowedSpecialChars(ALLOWED_NICK_SECIAL_CHARACTERS);
	for (int i = 0; nick[i] != '\0'; i++) {
		if (allowedSpecialChars.find(nick[i]) == string::npos && !isalpha(nick[i]) && !isdigit(nick[i])) {
			sendMessage(user->getFD(), ERR_ERRONEUSNICKNAME(_serverName, "", nick));
			return (false);
		}
	}
	for (userVector::iterator it = _usersPtr->begin(); it != _usersPtr->end(); it++) {
		if ((*it)->getNickname() == nick) {
			sendMessage(user->getFD(), ERR_NICKNAMEINUSE(_serverName, "", nick));
			return (false);
		}
	}
	return (true);
}
