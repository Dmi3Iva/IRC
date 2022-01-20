#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "ACommand.hpp"
#include "Channel.hpp"
#include "Commands/JoinCommand.hpp"
#include "Commands/NickCommand.hpp"
#include "Commands/PartCommand.hpp"
#include "Commands/PingCommand.hpp"
#include "Commands/PrivateMessageCommand.hpp"
#include "Commands/UserCommand.hpp"
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
	commandsMapType _commandsMap;
	vector<User*> _users;
	map<string, Channel*> _channels;
	string _serverName;

  void _setupCommands();
	int _executeCommand(User* user, string stringCommand);

public:
	Context();
	~Context();

  User* findUserByFd(int userfd);
  void addUser(User* user);
  void deleteUser(User* user);
  void _handleMessage(User* user);
};

#endif