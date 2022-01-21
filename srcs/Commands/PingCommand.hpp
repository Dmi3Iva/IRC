#ifndef PINGCOMMAND_HPP
#define PINGCOMMAND_HPP

#include "../ACommand.hpp"

class PingCommand : public ACommand {
public:
	PingCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr);
	void execute(User* user, string cmd);
};

#endif
