#include "JoinCommand.hpp"

JoinCommand::JoinCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr)
	: ACommand(serverName, usersPtr, channelsPtr)
{
	_name = "JOIN";
}

/**
 *
 * @param user
 */
void JoinCommand::_userHasJoinedChannel(User* user, channelMap::iterator chIterator)
{
	chIterator->second->sendToAllChannelMembers(
		JOIN_RPL(user->getNickname(), user->getUsername(), user->getHostname(), chIterator->second->getName()));
	// send channel topic to the user
	sendMessage(user->getFD(), RPL_TOPIC(_serverName, chIterator->second->getName(), chIterator->second->getTopic()));

	// send channel members to joined user
	if (chIterator->second->getMembers().empty()) {
		// if there aren't any members send empty response, but it actually seems to be wrong
		cerr << "ERROR: Channel " << chIterator->second->getName() << " after user joined still empty!" << endl;
		sendMessage(user->getFD(), RPL_NAMREPLY(_serverName, chIterator->second->getName(), ""));
	} else {
		for (Channel::usersVectorType::const_iterator it = chIterator->second->getMembers().begin(),
														ite = chIterator->second->getMembers().end();
			 it != ite; ++it)
			sendMessage(user->getFD(), RPL_NAMREPLY(_serverName, chIterator->second->getName(), (chIterator->second->isOperator(*it) ? "@" + (*it)->getNickname() : (*it)->getNickname())));
	}
	// end of sending
	sendMessage(user->getFD(), RPL_ENDOFNAMES(_serverName, chIterator->second->getName()));
}

void JoinCommand::_joinChannel(User* user, string channelName, string key)
{
	channelMap::iterator it = _channelsPtr->find(channelName);

	// if channel is not created, create it
	if (it == _channelsPtr->end()) {
		std::pair<channelMap::iterator, bool> newChannel = _channelsPtr->insert(make_pair(channelName, new Channel(channelName)));
		it = newChannel.first;
	}
	// else check channel and user
	else {
		if (user->isFullOfChannels()) {
			sendMessage(user->getFD(), ERR_TOOMANYCHANNELS(_serverName, user->getNickname(), channelName));
			return;
		}
		if (it->second->isFullOfMembers()) {
			sendMessage(user->getFD(), ERR_CHANNELISFULL(_serverName, user->getNickname(), channelName));
			return;
		}
		if (it->second->getPassword() != key) {
			sendMessage(user->getFD(), ERR_BADCHANNELKEY(_serverName, user->getNickname(), channelName));
			return;
		}
		if (it->second->isUserBanned(user)) {
			sendMessage(user->getFD(), ERR_BANNEDFROMCHAN(_serverName, user->getNickname(), channelName));
			return;
		}
		if (it->second->isOnlyInviteChannel()) {
			sendMessage(user->getFD(), ERR_INVITEONLYCHAN(_serverName, user->getNickname(), channelName));
			return;
		}
	}

	// add user to channel
	it->second->addUser(user);
	// add channel to user's channel list. Do nothing if user already in channel
	if (user->addChannel(it->second)) {
		// send all required messages
		_userHasJoinedChannel(user, it);
	}
}

/**
 * https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.1
 * @param user
 * @param cmd
 */
void JoinCommand::execute(User* user, string cmd)
{
	cout << "JOIN executes: " << cmd << endl;
	if (!user->getIsRegistered()) {
		sendMessage(
			user->getFD(),
			ERR_NOTREGISTERED(_serverName, (user->getNickname().empty() ? std::string("*") : user->getNickname()), _name));
		return;
	}
	vector<string> channelsAndKeys = ft_split(cmd, " ");
	// if there not only keys and channels then show error
	if (channelsAndKeys.size() < 1 || channelsAndKeys.size() > 2) {
		sendMessage(user->getFD(), ERR_NEEDMOREPARAMS(_serverName, user->getNickname(), _name));
		return;
	}
	vector<string> channels = ft_split(channelsAndKeys[0], ",");
	for (vector<string>::iterator it = channels.begin(), ite = channels.end(); it != ite; ++it)
		if (!isChannelName(*it)) {
			sendMessage(user->getFD(), ERR_NOSUCHCHANNEL(_serverName, user->getNickname(), _name));
			return;
		}
	vector<string> keys;
	if (channelsAndKeys.size() > 1)
		keys = ft_split(channelsAndKeys[1], ",");

	// if channels and keys lists have different sizes, last channels received empty keys
	for (size_t i = 0; i < channels.size(); ++i) {
		_joinChannel(user, channels[0], i < keys.size() ? keys[i] : string());
	}
}
