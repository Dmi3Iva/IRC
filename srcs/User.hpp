#ifndef USER_HPP
#define USER_HPP

#include "Channel.hpp"
#include "constants.hpp"
#include "utils.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netdb.h>
#include <netinet/in.h> // For sockaddr_in
#include <poll.h>
#include <sstream>
#include <string>
#include <sys/socket.h> // For socket functions
#include <unistd.h>
#include <utility>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::getline;
using std::make_pair;
using std::map;
using std::pair;
using std::string;
using std::vector;

class Context;
class Channel;

class User {
public:
	typedef map<string, Channel*> userChannels;

	User& operator=(const User& user);
	User(const User& user);
	User(int userFd, string hostname, string port, bool isAuthenticated);
	~User();

	int getFD() const;
	void setNickname(string nickname);
	void setUsername(string username);
	void setRealname(string realname);
	void setHostname(string hostname);
	void setMessage(string message);
	string getNickname() const;
	string getUsername() const;
	string getRealname() const;
	const string& getMessage() const;
	bool isRegistered() const;
	const string& getHostname() const;
	const string& getPort() const;
	void setIsRegistered(bool isRegistered);
	bool getIsNickPerformed() const;
	void setIsNickPerformed(bool value);

	bool getIsUserPerformed() const;
	void setIsUserPerformed(bool value);
	bool isAuthenticated() const;
	void setIsAuthenticated(bool isAuthenticated);

	bool addChannel(Channel* pChannel);
	int quitChannel(string channelName);
	bool isFullOfChannels();
	const userChannels& getChannels() const;
	bool isOper() const;
	bool isInvisible() const;
	void setIsInvisible(bool is_invisible);
	bool isReceiptNotice() const;
	void setIsReceiptNotice(bool is_receipt_notice);
	bool isReceivesWallops() const;
	void setIsReceivesWallops(bool is_receives_wallops);
	void setIsOper(bool is_oper);

private:
	int _fd;
	string _nickname;
	string _username;
	string _realname;
	string _hostname;
	string _port;
	bool _isRegistered;
	bool _isOper;
	bool _isNickPerformed;
	bool _isUserPerformed;
	bool _isAuthenticated;
	bool _isInvisible;
	bool _isReceivesWallops; // MODE w
	bool _isReceiptNotice; // MODE s
	string _message;
	userChannels _userChannels;
};

#endif
