#include "JoinCommand.hpp"

JoinCommand::JoinCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr)
	: ACommand(serverName, usersPtr, channelsPtr, "JOIN")
{
	_description = "JOIN <channel>{,<channel>} [<key>{,<key>}]";
}

/**
 *
 * @param user
 */
void JoinCommand::_userHasJoinedChannel(User* user, channelMap::iterator chIterator)
{
	chIterator->second->sendToAllChannelMembers(JOIN_RPL(user->getNickname(), user->getUsername(), user->getHostname(), chIterator->second->getName()));
	// send channel topic to the user
	if (chIterator->second->getTopic().empty())
		sendMessage(user->getFD(), RPL_NOTOPIC(_serverName, chIterator->second->getName()));
	else
		sendMessage(user->getFD(), RPL_TOPIC(_serverName, chIterator->second->getName(), chIterator->second->getTopic()));
	// send channel members to joined user
	for (Channel::usersVectorType::const_iterator it = chIterator->second->getMembers().begin(); it != chIterator->second->getMembers().end(); ++it) {
		if (!(*it)->isInvisible()) {
			chIterator->second->sendToAllChannelMembers(
				RPL_NAMREPLY(_serverName, chIterator->second->getNameWithPrefix(), user->getNickname(), chIterator->second->getUserPrefix(*it) + (*it)->getNickname()));
		}
	}
	// end of sending
	chIterator->second->sendToAllChannelMembers(RPL_ENDOFNAMES(_serverName, chIterator->second->getName(), user->getNickname()));
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
		if (it->second->isBanned(user)) {
			sendMessage(user->getFD(), ERR_BANNEDFROMCHAN(_serverName, user->getNickname(), channelName));
			return;
		}
		if (it->second->isInviteOnlyChannel()) {
			// if user isn't in invintation list then return else remove him from list and then add
			if (!it->second->isUserInInvintationList(user)) {
				sendMessage(user->getFD(), ERR_INVITEONLYCHAN(_serverName, user->getNickname(), channelName));
				return;
			} else {
				it->second->removeUserFromInvintationList(user);
			}
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
	if (!user->isRegistered()) {
		sendMessage(user->getFD(), ERR_NOTREGISTERED(_serverName, (user->getNickname().empty() ? std::string("*") : user->getNickname()), _name));
		return;
	}
	vector<string> channelsAndKeys = ft_split(cmd, " ");
	// if there not only keys and channels then show error
	if (channelsAndKeys.size() < 1 || channelsAndKeys.size() > 2) {
		sendMessage(user->getFD(), ERR_NEEDMOREPARAMS(_serverName, user->getNickname(), _name));
		return;
	}
	vector<string> channels = ft_split(channelsAndKeys[0], ",");
	vector<string> keys;
	if (channelsAndKeys.size() > 1)
		keys = ft_split(channelsAndKeys[1], ",");

	// if channels and keys lists have different sizes, last channels received empty keys
	for (size_t i = 0; i < channels.size(); ++i) {
		if (isChannelName(channels[i])) {
			_joinChannel(user, channels[i], i < keys.size() ? keys[i] : string());
		} else {
			sendMessage(user->getFD(), ERR_NOSUCHCHANNEL(_serverName, user->getNickname(), channels[i]));
		}
	}
}
