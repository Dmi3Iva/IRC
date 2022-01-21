#ifndef PARTCOMMAND_HPP
#define PARTCOMMAND_HPP

#include "../ACommand.hpp"
#include "../User.hpp"
#include "../responses.hpp"
#include "../utils.hpp"
#include <iostream>
#include <utility>

class ACommand;
class Context;
class User;

class PartCommand : public ACommand {
public:
	PartCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr);
	void execute(User* user, string cmd);
};

#endif
