#include "Bot.hpp"

Bot::Bot(string ip, string port, string pass)
	: _password(pass)
{
	_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_sock == -1) {
		return; // TODO:: exit
	}
	_address.sin_family = AF_INET;
	_address.sin_port = htons(atoi(port.c_str()));
	_address.sin_addr.s_addr = inet_addr(ip.c_str());

	// connect to the server on socket
	int connectResult = connect(_sock, (sockaddr*)&_address, sizeof(_address));
	if (connectResult == -1) {
		cout << "connection failed :" << errno << endl; // TODO:: exit
	}
	std::srand(std::time(0));
}

Bot::~Bot() { close(_sock); }

string Bot::_getMessage()
{
	ssize_t bytesReceived;
	char buffer[4096];
	string message;

	memset(buffer, 0, 4096);
	if ((bytesReceived = recv(_sock, buffer, 4096, 0)) > 0) {
		buffer[bytesReceived] = '\0';
		cout << "received new message" << buffer << endl;
		message.append(string(buffer));
	}
	return _parseMessage(message);
}

string Bot::_parseMessage(string message)
{
	string result = "";

	// remove all \r
	message.erase(std::remove(message.begin(), message.end(), '\r'), message.end());
	// add string to the remainder
	_remainder.append(message);
	// if remainder has end of the line stop symbol \n
	if (_remainder.find("\n") != string::npos) {
		// add string to result
		result.append(_remainder.substr(0, _remainder.rfind("\n")));
		// keep remainder of the message
		_remainder.erase(0, _remainder.rfind("\n") + 1);
	}
	cout << "Got new message: " << result << endl;
	return result;
}

void Bot::start()
{
	string response;

	_authorize();
	// TODO:: should I add here stop flag?
	while (true) {
		response = _responseToMessage(_getMessage());
		if (!response.empty())
			_sendMessage(response);
	}
}

void Bot::_authorize() { _sendMessage("PASS :1\nUSER bot * * :i'm bot\nNICK bot\n"); }

// :qwe!user@user PRIVMSG bot :sdfew
string Bot::_responseToMessage(string message)
{
	vector<string> args = ft_split(message, " ");
	if (args.size() > 3 && args[1] == "PRIVMSG") {
		return std::string("PRIVMSG ") + _getNickname(args[0]) + " :" + guessingAnswers[std::rand() % GUESSING_ANSWERS_COUNT] + DELIMITER;
	}
	return "";
}

// :qwe!user@user
string Bot::_getNickname(string s)
{
	if (!s.empty() && s[0] == ':')
		s.erase(0, 1);
	if (s.find("!") != string::npos)
		s.erase(s.find("!"), string::npos);
	return s;
}

void Bot::_sendMessage(string message)
{
	cout << "sending message: " << message << endl;
	int sendResult = send(_sock, message.c_str(), message.size(), 0); // Check result
	if (sendResult == -1) {
		cout << "cant' sernd to the server" << endl;
	}
}
