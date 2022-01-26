#ifndef JOINCOMMAND_HPP
#define JOINCOMMAND_HPP

#include "../ACommand.hpp"

class JoinCommand : public ACommand {
public:
	JoinCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr);
	void execute(User* user, string cmd);

private:
	void _joinChannel(User* user, string channelName, string key);
	void _userHasJoinedChannel(User* user, channelMap::iterator chIterator);
};

#endif
