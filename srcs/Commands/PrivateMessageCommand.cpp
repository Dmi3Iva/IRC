#include "PrivateMessageCommand.hpp"

PrivateMessageCommand::PrivateMessageCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr)
	: ACommand(serverName, usersPtr, channelsPtr, "PRIVMSG")
{
	_description = "PRIVMSG <receiver>{,<receiver>} <text to be sent>";
}

PrivateMessageCommand::~PrivateMessageCommand() { }

void PrivateMessageCommand::execute(User* user, string cmd)
{

	if (!user->isRegistered()) {
		user->appendBuffer(ERR_NOTREGISTERED(_serverName, user->getNickname(), "PRIVMSG"));
		//		sendMessage(user->getFD(), ERR_NOTREGISTERED(_serverName, user->getNickname(), "PRIVMSG"));
		return;
	}

	vector<string> receivers = _getReceivers(cmd);
	if (receivers.size() == 0 || receivers[0] == "" || receivers[0].c_str()[0] == ':') {
		user->appendBuffer(ERR_NORECIPIENT(_serverName, user->getNickname(), _name, _description));
//		sendMessage(user->getFD(), ERR_NORECIPIENT(_serverName, user->getNickname(), _name, _description));
		return;
	}

	string message = _constructMessage(cmd);

	if (message.size() == 0 || message == "") {
		user->appendBuffer(ERR_NOTEXTTOSEND(_serverName, user->getNickname()));
		//		sendMessage(user->getFD(), ERR_NOTEXTTOSEND(_serverName, user->getNickname()));
		return;
	}
	_sendMessageToReceivers(user, receivers, message);
}

string PrivateMessageCommand::_constructMessage(string cmd)
{
	size_t receiversLen = cmd.find(' ');
	if (receiversLen == string::npos) {
		return ("");
	}
	cmd.erase(0, receiversLen);
	eraseSpacesInFront(cmd);
	cmd.erase(remove(cmd.begin(), cmd.end(), ':'), cmd.end());
	return (cmd);
}

vector<string> PrivateMessageCommand::_getReceivers(string& cmd)
{
	vector<string> receivers;

	eraseSpacesInFront(cmd);
	size_t receiversLen = cmd.find(' ');
	if (receiversLen == string::npos) {
		return (receivers);
	}
	string receiversStr = cmd.substr(0, receiversLen);
	receivers = ft_split(receiversStr, ",");
	return (receivers);
}

void PrivateMessageCommand::_sendMessageToReceivers(User* user, vector<string>& receivers, string message)
{
	list<string> handledReceivers;

	for (vector<string>::iterator it = receivers.begin(); it < receivers.end(); it++) {
		if (isChannel(*it)) {
			channelMap::iterator channel = _channelsPtr->find(*it);
			if (channel != _channelsPtr->end()) {
				if (!channel->second->isUserCanSpeak(user)) {
					user->appendBuffer(ERR_CANNOTSENDTOCHAN(_serverName, user->getNickname(), *it));
//					sendMessage(user->getFD(), ERR_CANNOTSENDTOCHAN(_serverName, user->getNickname(), *it));
				} else if (_isReceiverAlredyGotMessage(handledReceivers, channel->second->getName())) {
					user->appendBuffer(ERR_TOOMANYTARGETS(_serverName, user->getNickname(), *it));
//					sendMessage(user->getFD(), ERR_TOOMANYTARGETS(_serverName, user->getNickname(), *it));
				} else {
					channel->second->sendToAllChannelMembers(RPL_PRIVMSG(user->getNickname(), user->getUsername(), user->getUsername(), *it, message), user);
					handledReceivers.push_back(channel->second->getName());
				}
			} else {
				user->appendBuffer(ERR_NOSUCHNICK(_serverName, user->getNickname(), *it));
//				sendMessage(user->getFD(), ERR_NOSUCHNICK(_serverName, user->getNickname(), *it));
			}
		} else {
			User* userReceiver = this->getUserFromArray(*it);
			if (userReceiver) {
				if (_isReceiverAlredyGotMessage(handledReceivers, userReceiver->getNickname())) {
					user->appendBuffer(ERR_TOOMANYTARGETS(_serverName, user->getNickname(), *it));
//					sendMessage(user->getFD(), ERR_TOOMANYTARGETS(_serverName, user->getNickname(), *it));
				} else {
					user->appendBuffer(RPL_PRIVMSG(user->getNickname(), user->getUsername(), user->getUsername(), *it, message));
//					sendMessage(userReceiver->getFD(), RPL_PRIVMSG(user->getNickname(), user->getUsername(), user->getUsername(), *it, message));
					handledReceivers.push_back(userReceiver->getNickname());
					if (userReceiver->getIsAway()) {
						user->appendBuffer(RPL_AWAY(_serverName, user->getNickname(), userReceiver->getNickname(), userReceiver->getAwayMessage()));
//						sendMessage(user->getFD(), RPL_AWAY(_serverName, user->getNickname(), userReceiver->getNickname(), userReceiver->getAwayMessage()));
					}
				}
			} else {
				user->appendBuffer(ERR_NOSUCHNICK(_serverName, user->getNickname(), *it));
//				sendMessage(user->getFD(), ERR_NOSUCHNICK(_serverName, user->getNickname(), *it));
			}
		}
	}
}

bool PrivateMessageCommand::_isReceiverAlredyGotMessage(list<string>& handledReceivers, string nick)
{
	for (list<string>::iterator it = handledReceivers.begin(); it != handledReceivers.end(); it++) {
		if (*it == nick) {
			return (true);
		}
	}
	return (false);
}