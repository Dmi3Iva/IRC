#ifndef NICKCOMMAND_HPP
#define NICKCOMMAND_HPP

#include "../ACommand.hpp"
#include "../Context.hpp"
#include "../responses.hpp"
#include <iostream>

class ACommand;

class NickCommand : public ACommand {
public:
	NickCommand(string serverName, vector<User*>* usersPtr, channelMap* channelsPtr);
	void execute(User* user, string cmd);
};

#endif