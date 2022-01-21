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

<<<<<<< HEAD
	void _setupCommands();
	int _executeCommand(User* user, string stringCommand);
=======
  void _setupCommands();
  int _executeCommand(User *user, string stringCommand);
>>>>>>> 47d11f854bbd249013107e56f971bae3c3de3713

public:
	Context();
	~Context();

<<<<<<< HEAD
	User* findUserByFd(int userfd);
	void addUser(User* user);
	void deleteUser(User* user);
	void handleMessage(User* user);
	void clearEmptyData();
=======
  User* findUserByFd(int userfd);
  void addUser(User* user);
  void deleteUser(User* user);
  void _handleMessage(User* user);
>>>>>>> 47d11f854bbd249013107e56f971bae3c3de3713
};

#endif