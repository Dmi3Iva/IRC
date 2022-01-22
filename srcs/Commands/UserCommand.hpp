#ifndef USERCOMMAND_HPP
#define USERCOMMAND_HPP

#include "../ACommand.hpp"
#include "../Context.hpp"
#include "../utils.hpp"
#include <iostream>
#include <vector>

class ACommand;
class Context;
class User;

class UserCommand : public ACommand {
public:
	UserCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr);
	void execute(User* user, string cmd);

private:
	string _constructRealnameAndEraseFromCmd(string& cmd, size_t colonPos);
};

#endif