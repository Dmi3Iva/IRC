#include "PrivateMessageCommand.hpp"

PrivateMessageCommand::PrivateMessageCommand(vector<User> *usersPtr, vector<Channel> *channelsPtr) :
	ACommand(usersPtr, channelsPtr) {
	
	_name = "PRIVMSG";
	_description = "PRIVMSG <receiver>{,<receiver>} <text to be sent>";
}

void	PrivateMessageCommand::execute(User *user, string cmd) {
	(void)user;
	cout << "CMD" << cmd << endl;
	cout << "PRIVMSG DO NOTHING FOR NOW" << std::endl;
}