#include "JoinCommand.hpp"

JoinCommand::JoinCommand(string serverName, userVector *usersPtr, channelMap *channelsPtr)
    : ACommand(serverName, usersPtr, channelsPtr) {
  _name = "JOIN";
}

/**
 * If a JOIN is successful, the user is then sent the channel's topic(using RPL_TOPIC)
 * and the list of users who are on the channel (using RPL_NAMREPLY), which must include the user joining.
 * @param user
 */
void JoinCommand::_userHasJoinedChannel(User *user, Channel *channel) {
  // resend user message to channel members
  for (Channel::usersVectorType::const_iterator it = channel->getMembers().begin(), ite = channel->getMembers().end();
       it != ite; ++it)
    sendMessage((*it)->getFD(),
                JOIN_REPL(user->getNickname(), user->getUsername(), user->getHostname(), channel->getName()));

  // send channel topic to the user
  sendMessage(user->getFD(), RPL_TOPIC(_serverName, channel->getName(), channel->getTopic()));

  // send channel members to joined user
  if (channel->getMembers().empty()) {
    // if there aren't any members send empty response, but it actually seems to be wrong
    cerr << "ERROR: Channel " << channel->getName() << " after user joined still empty!" << endl;
    sendMessage(user->getFD(), RPL_NAMREPLY(_serverName, channel->getName(), ""));
  } else {
    for (Channel::usersVectorType::const_iterator it = channel->getMembers().begin(), ite = channel->getMembers().end();
         it != ite; ++it)
      sendMessage(user->getFD(),
                  RPL_NAMREPLY(_serverName, channel->getName(),
                               (channel->isOperator(*it) ? "@" + (*it)->getNickname() : (*it)->getNickname())));
  }
  // end of sending
  sendMessage(user->getFD(), RPL_ENDOFNAMES(_serverName, channel->getName()));
}

void JoinCommand::_joinChannel(User *user, string channelName, string key) {
  channelMap::iterator it = _channelsPtr->find(channelName);

  // if there is no channel like this create new
  if (it == _channelsPtr->end()) {
    std::pair<channelMap::iterator, bool> newChannel =
        _channelsPtr->insert(make_pair(channelName, Channel(channelName)));
    it = newChannel.first;
  }
  // else check password, ban list
  else {
    if (user->isFullOfChannels()) {
      sendMessage(user->getFD(), ERR_TOOMANYCHANNELS(_serverName, user->getNickname(), channelName));
      return;
    }
    if (it->second.isFullOfMembers()) {
      sendMessage(user->getFD(), ERR_CHANNELISFULL(_serverName, user->getNickname(), channelName));
      return;
    }
    if (it->second.getPassword() != key) {
      sendMessage(user->getFD(), ERR_BADCHANNELKEY(_serverName, user->getNickname(), channelName));
      return;
    }
    if (isPUserInVector(user, it->second.getBannedUsers())) {
      sendMessage(user->getFD(), ERR_BANNEDFROMCHAN(_serverName, user->getNickname(), channelName));
      return;
    }
    if (it->second.isOnlyInviteChannel()) {
      sendMessage(user->getFD(), ERR_INVITEONLYCHAN(_serverName, user->getNickname(), channelName));
      return;
    }
  }

  // add user to channel
  it->second.addUser(user);
  // add channel to user's channel list
  user->addChannel(&it->second);
  // send all required messages

  _userHasJoinedChannel(user, &(it->second));
}

/**
 * https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.1
 * @param user
 * @param cmd
 */
void JoinCommand::execute(User *user, string cmd) {
  cout << "Execute command " << cmd << endl;
  // first we have to check if the user is authorized
  // if user not registered than send error message?
  if (!user->getIsRegistered()) {
    sendMessage(
        user->getFD(),
        ERR_NOTREGISTERED(_serverName, (user->getNickname().empty() ? std::string("*") : user->getNickname()), _name));
    return;
  }
  // split string into some
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

  // if channel and keys lists have different sizes, last channels received empty strings
  for (size_t i = 0; i < channels.size(); ++i) {
    _joinChannel(user, channels[0], i < keys.size() ? keys[i] : string());
  }
}
