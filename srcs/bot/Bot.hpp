//
// Created by Shandy Mephesto on 1/27/22.
//

#ifndef BOT_HPP
#define BOT_HPP

#include "../utils.hpp"
#include "config.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class Bot {
public:
	Bot(string, string, string);
	~Bot();
	void start();

private:
	int _sock;
	string _ip;
	struct sockaddr_in _address;
	string _port;
	string _password;
	string _remainder;

	string _getMessage();
	string _parseMessage(string);
	void _sendMessage(string message);
	void _authorize();
	string _responseToMessage(string message);
	string _getNickname(string s);
};

#endif // IRC_BOT_HPP
