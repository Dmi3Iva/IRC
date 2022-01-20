#ifndef PRIVATEMESSAGECOMMAND_HPP
# define PRIVATEMESSAGECOMMAND_HPP

#include "../ACommand.hpp"
#include "../responses.hpp"
#include "../Context.hpp"
#include <iostream>

class ACommand;

class PrivateMessageCommand : public ACommand {

public:
	PrivateMessageCommand(vector<User> *usersPtr, vector<Channel> *channelsPtr);
	virtual ~PrivateMessageCommand();

	void			execute(User *user, string cmd);

	vector<string>	getReceivers(string &cmd);
	string			getMSG(string cmd);
};

#endif