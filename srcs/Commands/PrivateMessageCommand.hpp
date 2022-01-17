#ifndef PRIVATEMESSAGECOMMAND_HPP
# define PRIVATEMESSAGECOMMAND_HPP

#include "../ACommand.hpp"
#include "../responses.hpp"
#include "../Context/Context.hpp"
#include <iostream>

class ACommand;

class PrivateMessageCommand : public ACommand {

public:
	PrivateMessageCommand(vector<User> *usersPtr, vector<Channel> *channelsPtr);
	void	execute(User *user, string cmd);
};

#endif