#include "PrivateMessageCommand.hpp"

PrivateMessageCommand::PrivateMessageCommand(vector<User> *usersPtr, vector<Channel> *channelsPtr) :
	ACommand(usersPtr, channelsPtr) {
	
	_name = "PRIVMSG";
	_description = "PRIVMSG <receiver>{,<receiver>} <text to be sent>";
}

void	PrivateMessageCommand::execute(User *user, string cmd) {
	cout << "CMD " << cmd << endl;
	cout << "PRIVMSG DO NOTHING FOR NOW" << std::endl;

	if (!user->getIsRegistered()) {
		cout << "484 * :Your connection is restricted!" << endl;
		return ;
	}

	string	receiversStr = cmd.substr(0, cmd.find(' '));
	// cout << endl << "Receivers " << receiversStr << endl << "Other cmd " << cmd << endl << endl;
	vector<string>	receivers = ft_split(receiversStr, ",");
	// cout << endl << "Receivers: " << endl;
	// for (size_t i = 0; i < receivers.size(); i++) {
	// 	cout << receivers[i] << endl;
	// }
	// cout << endl;
}