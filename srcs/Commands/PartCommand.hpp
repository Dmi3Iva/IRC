#ifndef PARTCOMMAND_HPP
#define PARTCOMMAND_HPP

#include "../ACommand.hpp"

class PartCommand : public ACommand {
public:
	PartCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr);
	void execute(User* user, string cmd);
};

#endif
