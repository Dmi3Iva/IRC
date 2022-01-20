#include "PrivateMessageCommand.hpp"

PrivateMessageCommand::PrivateMessageCommand(vector<User> *usersPtr, vector<Channel> *channelsPtr) :
		ACommand(usersPtr, channelsPtr) {
	
	_name = "PRIVMSG";
	_description = "PRIVMSG <receiver>{,<receiver>} <text to be sent>";
}

PrivateMessageCommand::~PrivateMessageCommand() {}

void	PrivateMessageCommand::execute(User *user, string cmd) {
	cout << "CMD " << cmd << endl;
	cout << "PRIVMSG DO NOTHING FOR NOW" << std::endl;

	if (!user->getIsRegistered()) {
		cout << "484 * :Your connection is restricted!" << endl;
		return ;
	}

	vector<string> receivers = getReceivers(cmd);
	if (receivers.size() == 0 || receivers[0] == "" || receivers[0].c_str()[0] == ':') {
		cout << "401 * :No such nick/channel" << endl;
		return ;
	}

	string message = getMSG(cmd);

	for (vector<string>::iterator it = receivers.begin(); it < receivers.end(); it++) {
		User	*userReceiver = this->getUserFromArray(*it);
		if (userReceiver) {
			this->sendMessage(userReceiver->getFD(), RPL_PRIVMSG(user->getNickname(),
																user->getUsername(),
																user->getUsername(),
																*it,
																message));
		}
	}
}

string			PrivateMessageCommand::getMSG(string cmd) {
	size_t receiversLen = cmd.find(' ');
	cmd.erase(0, receiversLen);

	char	*cmd_char = (char*)cmd.c_str();
	int		i = 0;
	while (cmd_char[i] && cmd_char[i] == ' ')
		i++;

	cmd.erase(0, i);
	cmd.erase(remove(cmd.begin(), cmd.end(), ':'), cmd.end());
	// cout << "MSG is " << cmd << endl;
	return (cmd);
}

vector<string>	PrivateMessageCommand::getReceivers(string &cmd) {
	string	receiversStr = cmd.substr(0, cmd.find(' '));
	cout << endl << "Receivers " << receiversStr << endl << "Other cmd " << cmd << endl << endl;
	vector<string>	receivers = ft_split(receiversStr, ",");
	// cout << endl << "Receivers: " << endl;
	// for (size_t i = 0; i < receivers.size(); i++) {
	// 	cout << receivers[i] << endl;
	// }
	// cout << endl;
	return (receivers);
}