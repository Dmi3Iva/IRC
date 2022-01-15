#include "User.hpp"

User::User(int userFd, string hostname, string port)
    : _fd(userFd), _hostname(hostname), _port(port), _isRegistered(false) {}

User::~User() {}

void User::closeFD() { close(_fd); }

int User::getFD() { return _fd; }

void User::setNickname(string nickname) {
  cout << "set nickname: " << nickname << endl;
  _nickname = nickname;
}

void User::setUsername(string username) {
  cout << "set username: " << username << endl;
  _username = username;
}

void User::setRealname(string realname) {
  cout << "set realname: " << realname << endl;
  _realname = realname;
}

bool User::getIsRegistered() { return _isRegistered; }

void User::setIsRegistered(bool isRegistered) { _isRegistered = isRegistered; }

string User::getNickname() { return _nickname; }

string User::getUsername() { return _username; }

string User::getRealname() { return _realname; }

void User::addChannel(Channel *pChannel) {
  pair<userChannels::iterator, bool> result = _channels.insert(make_pair(pChannel->getName(), pChannel));
  if (result.second) {
    cout << "User:: Channel added to user list successfully" << endl;
  } else {
    cout << "User:: Channel already in user list!" << endl;
  }
}

int User::quitChannel(string channelName) {
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

const string &User::getHostname() const { return _hostname; }

const string &User::getPort() const { return _port; }

int User::getMaxOfChannels() const { return _MAX_OF_CHANNELS; }

bool User::isFullOfChannels() { return _channels.size() >= _MAX_OF_CHANNELS; }
