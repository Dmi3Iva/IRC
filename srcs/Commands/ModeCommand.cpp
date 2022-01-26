#include "ModeCommand.hpp"

ModeCommand::ModeCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr)
	: ACommand(serverName, usersPtr, channelsPtr, "MODE")
{
	_description = "<channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>] [<ban mask>] or <nickname> {[+|-]|i|w|s|o}";
}

/*
ERR_NEEDMOREPARAMS "<command> :Not enough parameters"
RPL_CHANNELMODEIS "<channel> <mode> <mode params>"
ERR_CHANOPRIVSNEEDED "<channel> :You're not channel operator"

 - Any command requiring 'chanop' privileges (such as
MODE messages) must return this error if the client
making the attempt is not a chanop on the specified
channel.

ERR_NOSUCHNICK "<nickname> :No such nick/channel"
ERR_NOTONCHANNEL "<channel> :You're not on that channel"

- Returned by the server whenever a client tries to
  perform a channel effecting command for which the
  client isn't a member.

ERR_KEYSET "<channel> :Channel key already set"
RPL_BANLIST  "<channel> <banid>"
RPL_ENDOFBANLIST "<channel> :End of channel ban list"

- When listing the active 'bans' for a given channel,
  a server is required to send the list back using the
  RPL_BANLIST and RPL_ENDOFBANLIST messages.  A separate
  RPL_BANLIST is sent for each active banid.  After the
  banids have been listed (or if none present) a
  RPL_ENDOFBANLIST must be sent.

ERR_UNKNOWNMODE ERR_UNKNOWNMODE
"<char> :is unknown mode char to me"

ERR_NOSUCHCHANNEL
 "<channel name> :No such channel"

- Used to indicate the given channel name is invalid.

ERR_USERSDONTMATCH ":Cant change mode for other users"

- Error sent to any user trying to view or change the
  user mode for a user other than themselves.
RPL_UMODEIS "<user mode string>" - To answer a query about a client's own mode,
RPL_UMODEIS is sent back.
ERR_UMODEUNKNOWNFLAGG  ":Unknown MODE flag"

- Returned by the server to indicate that a MODE
		message was sent with a nickname parameter and that
			the a mode flag sent was not recognized.
 */

/*

 Common::
ERR_NEEDMOREPARAMS // need more params
ERR_NOSUCHNICK // no such nick
ERR_UNKNOWNMODE // <char> :is unknown mode char to me
ERR_UMODEUNKNOWNFLAG // Returned by the server to indicate that a MODE message was sent with a nickname parameter and that the a mode flag sent was not
recognized. CHANNEL:: ERR_CHANOPRIVSNEEDED // you're not channel operator ERR_NOTONCHANNEL // You're not on that channel ERR_KEYSET // Channel key already set
ERR_NOSUCHCHANNEL // "<channel name> :No such channel"
 USER::
ERR_USERSDONTMATCH // Error sent to any user trying to view or change the user mode for a user other than themselves.

RPL_UMODEIS
RPL_BANLIST
RPL_ENDOFBANLIST
RPL_CHANNELMODEIS
 */
void ModeCommand::execute(User* user, string cmd)
{
	if (!user->isRegistered()) {
		sendMessage(user->getFD(), ERR_NOTREGISTERED(_serverName, (user->getNickname().empty() ? std::string("*") : user->getNickname()), _name));
		return;
	}

	size_t firstSpace = cmd.find(' ');
	if (firstSpace == string::npos) {
		sendMessage(user->getFD(), ERR_NEEDMOREPARAMS(_serverName, user->getNickname(), _name));
		return;
	}
	string targetName = cmd.substr(0, firstSpace);
	string arguments = trim(cmd.substr(firstSpace + 1));
	if (targetName.empty() || arguments.empty()) {
		sendMessage(user->getFD(), ERR_NEEDMOREPARAMS(_serverName, user->getNickname(), _name));
		return;
	}

	if (isChannelName(targetName)) {
		channelMap::iterator chIterator = _channelsPtr->find(targetName);
		if (chIterator == _channelsPtr->end()) {
			sendMessage(user->getFD(), ERR_NOSUCHNICK(_serverName, user->getNickname()));
			return;
		}
		_executeChannelMod(user, chIterator->second, arguments);
	} else {
		User* userTarget = getUserFromArray(targetName);
		if (userTarget == NULL) {
			sendMessage(user->getFD(), ERR_NOSUCHNICK(_serverName, user->getNickname()));
			return;
		}
		// all what goes after first argument erase
		_executeUserMod(user, userTarget, arguments.substr(0, arguments.find(" ")));
	}
}

/**
 * https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.3.1
 * <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>] [<ban mask>]
 * there is a strange order of arguments. Just take arguments in required for flags order
 * @param user
 * @param channel
 * @param arguments
 */
void ModeCommand::_executeChannelMod(User* user, Channel* channel, string arguments)
{
	bool isPlus;
	size_t spacePos = arguments.find(' ');
	string changes;
	string modes = arguments.substr(0, spacePos);
	deque<string> optionalArguments = ft_split<deque<string> >(spacePos == string::npos ? "" : arguments.substr(spacePos), " ");

	if (!channel->isUserMember(user)) {
		sendMessage(user->getFD(), ERR_NOTONCHANNEL(_serverName, user->getNickname(), channel->getName()));
		return;
	}
	if (!channel->isOperator(user)) {
		sendMessage(user->getFD(), ERR_CHANOPRIVSNEEDED(_serverName, user->getNickname(), channel->getName()));
		return;
	}
	if (string("+-").find(arguments[0]) == string::npos) {
		sendMessage(user->getFD(), ERR_UNKNOWNMODE(_serverName, user->getNickname(), string(1, arguments[0])));
		return;
	}
	isPlus = arguments[0] == '+';
	changes = string(1, arguments[0]);
	for (size_t i = 1; i < modes.size(); ++i) {
		switch (modes[i]) {
		case 'o':
			_handleOFlag(user, channel, isPlus, optionalArguments);
			break;
		case 'p':
			_handleCommonFlag(user, channel, isPlus, &Channel::setIsPrivate, channel->isPrivate(), "p", changes);
			break;
		case 's':
			_handleCommonFlag(user, channel, isPlus, &Channel::setIsSecret, channel->isSecret(), "s", changes);
			break;
		case 'i':
			_handleCommonFlag(user, channel, isPlus, &Channel::setIsInviteOnlyChannel, channel->isInviteOnlyChannel(), "i", changes);
			break;
		case 't':
			_handleCommonFlag(user, channel, isPlus, &Channel::setIsTopicSettableOnlyByOpers, channel->isTopicSettableOnlyByOpers(), "t", changes);
			break;
		case 'n':
			_handleCommonFlag(user, channel, isPlus, &Channel::setIsNoMessageOutside, channel->isNoMessageOutside(), "n", changes);
			break;
		case 'm':
			_handleCommonFlag(user, channel, isPlus, &Channel::setIsModerated, channel->isModerated(), "n", changes);
			break;
		case 'l':
			_handleLFlag(user, channel, getPopFront(optionalArguments), isPlus, changes);
			break;
		case 'b':
			_handleBFLag(user, channel, isPlus, getPopFront(optionalArguments));
			break;
		case 'k':
			_handleKFlag(user, channel, getPopFront(optionalArguments), changes, isPlus);
			break;
		case 'v':
			_handleVFlag(user, channel, getPopFront(optionalArguments), isPlus);
			break;
		default:
			sendMessage(user->getFD(), ERR_UNKNOWNMODE(_serverName, user->getNickname(), arguments[i]));
			break;
		}
	}
	if (changes.size() > 1) {
		channel->sendToAllChannelMembers(MODE_RPL(user->getNickname(), user->getUsername(), user->getHostname(), channel->getName(), changes));
	}
}

void ModeCommand::_handleOFlag(User* user, Channel* channel, bool isPlus, deque<string> optionalArguments)
{
	User* userTarget;

	if (optionalArguments.empty()) {
		sendMessage(user->getFD(), ERR_NEEDMOREPARAMS(_serverName, user->getNickname(), _name));
	} else if (!(userTarget = getUserFromArray(getPopFront(optionalArguments))) // if there aren't any user with this nickname
		|| !channel->isUserMember(userTarget)) { // if this user not channel member
		sendMessage(user->getFD(), ERR_NOSUCHNICK(_serverName, userTarget->getNickname()));
	} else if ((isPlus ? channel->addOper(userTarget) //
					   : channel->removeOper(userTarget) //
				   )) {
		channel->sendToAllChannelMembers(
			MODE_RPL(user->getNickname(), user->getUsername(), user->getHostname(), channel->getName(), (isPlus ? "+o :" : "-o :") + userTarget->getNickname()));
	} else {
		sendMessage(user->getFD(), ERR_KEYSET(_serverName, user->getNickname(), channel->getName()));
	}
}

void ModeCommand::_handleCommonFlag(User* user, Channel* channel, bool isPlus, void (Channel::*setValue)(bool), bool isValue, string key, string& changes)
{
	if (isValue != isPlus) {
		((*channel).*setValue)(isPlus);
		changes += key;
	} else {
		sendMessage(user->getFD(), ERR_KEYSET(_serverName, user->getNickname(), channel->getName()));
	}
}

void ModeCommand::_handleLFlag(User* user, Channel* channel, string argument, bool isPlus, string& changes)
{
	int l;

	if (isPlus) {
		if (argument.empty()) {
			sendMessage(user->getFD(), ERR_NEEDMOREPARAMS(_serverName, user->getNickname(), _name));
		} else {
			l = stoi(argument);
			if (l == channel->getUsersLimit()) {
				sendMessage(user->getFD(), ERR_KEYSET(_serverName, user->getNickname(), channel->getName()));
			} else {
				channel->setUsersLimit(l);
				channel->sendToAllChannelMembers(MODE_RPL(user->getNickname(), user->getUsername(), user->getHostname(), channel->getName(), "+l :" + std::to_string(l)));
			}
		}
	} else {
		channel->setUsersLimit(-1);
		changes += "l";
	}
}

void ModeCommand::_handleBFLag(User* user, Channel* channel, bool isPlus, string argument)
{
	if (argument.empty()) {
		sendMessage(user->getFD(), ERR_NEEDMOREPARAMS(_serverName, user->getNickname(), _name));
	} else {
		if (isPlus ? channel->addBannerMask(argument) //
				   : channel->removeBannerMask(argument) //
		) {
			channel->sendToAllChannelMembers(MODE_RPL(user->getNickname(), user->getUsername(), user->getHostname(), channel->getName(), (isPlus ? "+b :" : "-b :") + argument));
		} else {
			if (channel->isBannedMask(argument)) {
				// already using mask
				sendMessage(user->getFD(), ERR_KEYSET(_serverName, user->getNickname(), channel->getName()));
			} else {
				sendMessage(user->getFD(), ERR_NOSUCHNICK(_serverName, user->getNickname()));
			}
		}
	}
}

void ModeCommand::_handleKFlag(User* user, Channel* channel, string argument, string& changes, bool isPlus)
{
	if (argument.empty()) {
		if (isPlus) {
			sendMessage(user->getFD(), ERR_NEEDMOREPARAMS(_serverName, user->getNickname(), _name));
		} else {
			channel->setPassword("");
			changes += 'k';
		}
	} else if (channel->getPassword() != argument) {
		channel->setPassword(argument);
		changes += 'k';
	} else {
		sendMessage(user->getFD(), ERR_KEYSET(_serverName, user->getNickname(), channel->getName()));
	}
}

void ModeCommand::_handleVFlag(User* user, Channel* channel, string argument, bool isPlus)
{
	User* userTarget;

	if (argument.empty()) {
		sendMessage(user->getFD(), ERR_NEEDMOREPARAMS(_serverName, user->getNickname(), _name));
	} else {
		userTarget = getUserFromArray(argument);
		if (!userTarget || !channel->isUserMember(userTarget)) {
			sendMessage(user->getFD(), ERR_NOSUCHNICK(_serverName, userTarget->getNickname()));
		} else {
			if (isPlus ? channel->addSpeaker(userTarget) //
					   : channel->removeSpeaker(userTarget) //
			) {
				channel->sendToAllChannelMembers(
					MODE_RPL(user->getNickname(), user->getUsername(), user->getHostname(), channel->getName(), (isPlus ? "+v :" : "-v :") + userTarget->getNickname()));
			} else {
				sendMessage(user->getFD(), ERR_KEYSET(_serverName, user->getNickname(), channel->getName()));
			}
		}
	}
}

/**
 * https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.3.2
 * <nickname> {[+|-]|i|w|s|o}
 * @param user
 * @param userTarget
 * @param arguments
 */
void ModeCommand::_executeUserMod(User* user, User* userTarget, string arguments)
{
	bool isPlus;
	string changes;

	if (!user->isOper() && user->getNickname() != userTarget->getNickname()) {
		sendMessage(user->getFD(), ERR_USERSDONTMATCH(_serverName, user->getNickname()));
		return;
	}
	if (string("+-").find(arguments[0]) == string::npos) {
		sendMessage(user->getFD(), ERR_UNKNOWNMODE(_serverName, user->getNickname(), string(1, arguments[0])));
		return;
	}

	isPlus = arguments[0] == '+';
	changes = string(1, arguments[0]);
	for (size_t i = 1; i < arguments.size(); ++i) {
		switch (arguments[i]) {
		case 'i':
			userTarget->setIsInvisible(isPlus);
			changes += "i";
			break;
		case 'w':
			userTarget->setIsReceivesWallops(isPlus);
			changes += "w";
			break;
		case 's':
			userTarget->setIsReceiptNotice(isPlus); // this flag uses for server notices
			changes += "s";
			break;
		case 'o':
			userTarget->setIsOper(isPlus);
			changes += "o";
			break;
		default:
			sendMessage(user->getFD(), ERR_UMODEUNKNOWNFLAG(_serverName, user->getNickname()));
			break;
		}
	}
	if (changes.size() > 1) {
		_sendMessageToUserAndHisChannels(userTarget, MODE_RPL(user->getNickname(), user->getUsername(), user->getHostname(), userTarget->getNickname(), changes));
	}
}

void ModeCommand::_sendMessageToUserAndHisChannels(User* userTarget, string message)
{
	sendMessage(userTarget->getFD(), message);
	for (channelMap::const_iterator it = userTarget->getChannels().begin(), ite = userTarget->getChannels().end(); it != ite; ++it) {
		it->second->sendToAllChannelMembers(message, userTarget);
	}
}
