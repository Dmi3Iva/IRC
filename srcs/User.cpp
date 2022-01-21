#include "User.hpp"

<<<<<<< HEAD
User::User(int userFd, string hostname, string port)
	: _fd(userFd)
	, _hostname(hostname)
	, _port(port)
	, _isRegistered(false)
{
}

User& User::operator=(const User& user)
{
	_fd = user.getFD();
	_nickname = user.getNickname();
	_realname = user.getRealname();
	_hostname = user.getHostname();
	_port = user.getHostname();
	_isRegistered = user.isRegistered();
	_channels = user.getChannels();
	return *this;
=======
User::User(int userFd)
    :_fd(userFd),
     _isRegistered(false) {}

User::~User() {}

int User::getFD() { return _fd; }

void User::setNickname(string nickname) {
  cout << "set nickname: " << nickname << endl;
  _nickname = nickname;
>>>>>>> 47d11f854bbd249013107e56f971bae3c3de3713
}

User::User(const User& user) { *this = user; }

User::~User() { }

int User::getFD() const { return _fd; }

void User::setNickname(string nickname) { _nickname = nickname; }

void User::setUsername(string username) { _username = username; }

void User::setRealname(string realname) { _realname = realname; }

void User::setMessage(string message) { _message = message; }

void User::appendMessage(string message) { _message.append(message); }

bool User::isRegistered() const { return _isRegistered; }

void User::setIsRegistered(bool isRegistered) { _isRegistered = isRegistered; }

string User::getNickname() const { return _nickname; }

string User::getUsername() const { return _username; }

string User::getRealname() const { return _realname; }

const string& User::getMessage() const { return _message; }

/**
 * Add the user to the channel
 * @param pChannel
 * @return true if the user was added, false if user already in the channel
 */
bool User::addChannel(Channel* pChannel)
{
	pair<userChannels::iterator, bool> result = _channels.insert(make_pair(pChannel->getName(), pChannel));
	if (!result.second) {
		cout << "User->" << _nickname << ": channel " << pChannel->getName() << " already in user list!" << endl;
		return false;
	}
	return true;
}

int User::quitChannel(string channelName)
{
	userChannels::iterator chIterator = _channels.find(channelName);
	if (chIterator != _channels.end()) {
		chIterator->second->removeUser(this);
		_channels.erase(chIterator);
		return 1;
	}
	cout << "User->" << _nickname << " : "
		 << " doesn't have this channel in his/her list" << endl;
	return 0;
}

<<<<<<< HEAD
const string& User::getHostname() const { return _hostname; }
=======
void User::setMessage(string message) {
  cout << "set message: " << message << endl;
  _message = message;
}

void User::appendMessage(string message) {
  cout << "append message: " << message << endl;
  _message.append(message);
}

bool User::isRegistered() { return _isRegistered; }
>>>>>>> 47d11f854bbd249013107e56f971bae3c3de3713

const string& User::getPort() const { return _port; }

int User::getMaxOfChannels() const { return _MAX_OF_CHANNELS; }

<<<<<<< HEAD
bool User::isFullOfChannels() { return _channels.size() >= _MAX_OF_CHANNELS; }
const User::userChannels& User::getChannels() const { return _channels; }
=======
string User::getNickname() { return _nickname; }
string User::getUsername() { return _username; }
string User::getRealname() { return _realname; }
string User::getMessage() { return _message; }
>>>>>>> 47d11f854bbd249013107e56f971bae3c3de3713
