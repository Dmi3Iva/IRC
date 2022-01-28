#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "ACommand.hpp"
#include "Channel.hpp"
#include "Commands/AwayCommand.hpp"
#include "Commands/JoinCommand.hpp"
#include "Commands/NamesCommand.hpp"
#include "Commands/ModeCommand.hpp"
#include "Commands/NamesCommand.hpp"
#include "Commands/NickCommand.hpp"
#include "Commands/NoticeCommand.hpp"
#include "Commands/OperCommand.hpp"
#include "Commands/PartCommand.hpp"
#include "Commands/PassCommand.hpp"
#include "Commands/PingCommand.hpp"
#include "Commands/PrivateMessageCommand.hpp"
#include "Commands/UserCommand.hpp"
#include "Commands/WhoCommand.hpp"
#include "Commands/AwayCommand.hpp"
#include "Commands/NoticeCommand.hpp"
#include "Commands/KickCommand.hpp"
#include "Commands/WhoisCommand.hpp"
#include "Commands/InviteCommand.hpp"
#include "Commands/ListCommand.hpp"
#include "User.hpp"
#include "utils.hpp"
#include <map>
#include <utility>

using std::cout;
using std::endl;
using std::make_pair;
using std::map;
using std::string;
using std::vector;

class Context {
private:
	typedef map<string, ACommand*> commandsMapType;
	const string _serverName;
	const string _serverPassword;
	commandsMapType _commandsMap;
	vector<User*> _users;
	map<string, Channel*> _channels;

	void _setupCommands();
	int _executeCommand(User* user, string stringCommand);
	string _parseMessage(User* user, string buffer);

public:
	Context(const string serverName, const string serverPassword);
	~Context();

	const string& getServerName() const;
	const string& getServerPassword() const;
	User* findUserByFd(int userfd);
	void addUser(User* user);
	void deleteUser(User* user);
	void handleMessage(User* user, string message);
	void clearEmptyData();
	bool isPasswordSet();
};

#endif