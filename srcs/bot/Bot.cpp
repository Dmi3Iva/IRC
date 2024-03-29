#include "Bot.hpp"

Bot::Bot(string ip, string port, string pass)
	: _password(pass)
	, _stopFlag(false)
{
	_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_sock == -1) {
		cerr << "Unable create a socket, errno number:" << errno << endl;
		exit(EXIT_FAILURE);
	}
	_address.sin_family = AF_INET;
	_address.sin_port = getValidPort(port);
	if (inet_aton(ip.c_str(), &_address.sin_addr) == 0) {
		cout << "ERROR: Invalid ip address!" << endl;
		exit(EXIT_FAILURE);
	}
	// connect to the server on socket
	int connectResult = connect(_sock, (sockaddr*)&_address, sizeof(_address));
	if (connectResult == -1) {
		cout << "Unable to create connection, errno number :" << errno << endl;
		exit(EXIT_FAILURE);
	}
	std::srand(time(0));
}

Bot::~Bot() { close(_sock); }

string Bot::_getMessage()
{
	ssize_t bytesReceived;
	string buffer;
	string message;

	buffer.resize(4096, 0);
	if ((bytesReceived = recv(_sock, reinterpret_cast<void*>(const_cast<char*>(buffer.c_str())), 4096, 0)) > 0) {
		buffer[bytesReceived] = '\0';
		cout << "received new message" << buffer << endl;
		message.append(string(buffer));
	} else {
		setStopFlag(true);
		return "";
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

	setStopFlag(false);
	_authorize();
	while (!_stopFlag) {
		_sendMessage(_responseToMessage(_getMessage()));
	}
}

void Bot::_authorize() { _sendMessage("PASS :" + _password + "\nUSER " + BOT_USERNAME + " * * :" + BOT_REALNAME + "\nNICK " + BOT_USERNAME "\n"); }

string Bot::_responseToMessage(string message)
{
	if (message.empty())
		return "";
	// Remove nickname and check password error message
	if (message.substr(message.find(" ")).find("461 * PASS :Not enough parameters.") != string::npos) {
		cerr << "Incorrect password try again" << endl;
		exit(EXIT_FAILURE);
	}
	vector<string> args = ft_split(message, " ");
	if (args.size() > 3 && args[1] == "PRIVMSG") {
		if (args[3] == ":HELP" || args[3] == "HELP")
			return std::string("PRIVMSG ") + _getNickname(args[0]) + " :I'm future taller, ask me about anything! I will asnwer shortly." + DELIMITER;
		return std::string("PRIVMSG ") + _getNickname(args[0]) + " :" + guessingAnswers[std::rand() % GUESSING_ANSWERS_COUNT] + DELIMITER;
	}
	return "";
}

string Bot::_getNickname(string s)
{
	if (!s.empty() && s.find(":") != string::npos) {
		s.erase(0, s.find(":") + 1);
	}
	if (s.find("!") != string::npos)
		s.erase(s.find("!"), string::npos);
	return s;
}

void Bot::_sendMessage(string message)
{
	if (message.empty())
		return;
	cout << "sending message: " << message << endl;
	int sendResult = send(_sock, message.c_str(), message.size(), 0); // Check result
	if (sendResult == -1) {
		cout << "cant' sernd to the server" << endl;
	}
}

void Bot::setStopFlag(bool value)
{
	cout << "Set stop flag = " << value << endl;
	_stopFlag = value;
}
