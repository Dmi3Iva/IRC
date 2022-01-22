#ifndef OPERCOMMAND_HPP
#define OPERCOMMAND_HPP

#include "../ACommand.hpp"
/**
 * https://datatracker.ietf.org/doc/html/rfc1459#section-4.1.5
 */
class OperCommand : public ACommand {
	public:
	OperCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr);
	void execute(User* user, string cmd);
};

#endif
