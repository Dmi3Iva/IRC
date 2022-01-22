#include "User.hpp"

User::User(int userFd, string hostname, string port)
	: _fd(userFd)
	, _hostname(hostname)
	, _port(port)
	, _isRegistered(false)
	, _isNickPerformed(false)
	, _isUserPerformed(false)
	, _message("")
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
	_userChannels = user.getChannels();
	return *this;
}

User::User(const User& user) { *this = user; }

User::~User()
{
	cout << "User destructor was called nickname:" << _nickname << endl;
	close(_fd);
}

int User::getFD() const { return _fd; }

void User::setNickname(string nickname) { _nickname = nickname; }

void User::setUsername(string username) { _username = username; }

void User::setRealname(string realname) { _realname = realname; }

void User::setHostname(string hostname) { _hostname = hostname; }

void User::setMessage(string message) { _message = message; }

bool User::isRegistered() const { return _isRegistered; }

void User::setIsRegistered(bool isRegistered) { _isRegistered = isRegistered; }

bool User::getIsNickPerformed() const { return _isNickPerformed; }

void User::setIsNickPerformed(bool value) { _isNickPerformed = value; }

bool User::getIsUserPerformed() const { return _isUserPerformed; }

void User::setIsUserPerformed(bool value) { _isUserPerformed = value; }

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
	pair<userChannels::iterator, bool> result = _userChannels.insert(make_pair(pChannel->getName(), pChannel));
	if (!result.second) {
		cout << "User->" << _nickname << ": channel " << pChannel->getName() << " already in user list!" << endl;
		return false;
	}
	return true;
}

int User::quitChannel(string channelName)
{
	userChannels::iterator chIterator = _userChannels.find(channelName);
	if (chIterator != _userChannels.end()) {
		chIterator->second->removeUser(this);
		_userChannels.erase(chIterator);
		return 1;
	}
	cout << "User->" << _nickname << " : "
		 << " doesn't have this channel in his/her list" << endl;
	return 0;
}

const string& User::getHostname() const { return _hostname; }

const string& User::getPort() const { return _port; }

bool User::isFullOfChannels() { return _userChannels.size() >= USER_MAX_CHANNELS_COUNT; }

const User::userChannels& User::getChannels() const { return _userChannels; }
