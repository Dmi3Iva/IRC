#include "Channel.hpp"

Channel::Channel(const string name)
    : _name(name), _topic("default topic: welcome to the channel! :)"), _isOnlyInviteChannel(false), _owner(NULL),
      _usersLimit(-1) {}

Channel::~Channel() { cout << "Channel " << _name << " is closed" << endl; }

const string &Channel::getName() const { return _name; }

const string &Channel::getPassword() const { return _password; }

void Channel::setPassword(const string password) { _password = password; }

void Channel::addUser(User *p_user) {
  _members.push_back(p_user);
  if (!_owner) {
    _owner = p_user;
    _operators.push_back(p_user);
  }
}
const string &Channel::getTopic() const { return _topic; }

void Channel::setTopic(const string &topic) { _topic = topic; }

const Channel::usersVectorType &Channel::getMembers() const { return _members; }

const Channel::usersVectorType &Channel::getOperators() const { return _operators; }

void Channel::removeUser(User *pUser) {
  // only one member in the channel
  if (_members.size() == 1) {
    this->~Channel();
  } else {
    // remove user from _members by nick
    _removeUserByNickname(_members, pUser->getNickname());
    // remove user from _operators by nick
    _removeUserByNickname(_operators, pUser->getNickname());
    // if it's owner we have to find new
    if (pUser->getNickname() == _owner->getNickname()) {
      if (!_operators.empty()) {
        _owner = _operators.front();
      } else {
        _owner = _members.front();
        _operators.push_back(_owner);
      }
    }
  }
}

int Channel::_removeUserByNickname(Channel::usersVectorType &users, string nickname) {
  for (usersVectorType::iterator it = users.begin(), ite = users.end(); it != ite; ++it) {
    if ((*it)->getNickname() == nickname) {
      users.erase(it);
      return 1;
    }
  }
  return 0;
}

int Channel::isOperator(User *pUser) const {
  for (usersVectorType::const_iterator it = _operators.begin(), ite = _operators.end(); it != ite; ++it) {
    if ((*it)->getNickname() == pUser->getNickname())
      return 1;
  }
  return 0;
}
const Channel::usersVectorType &Channel::getBannedUsers() const { return _bannedUsers; }

bool Channel::isOnlyInviteChannel() const { return _isOnlyInviteChannel; }

void Channel::setIsOnlyInviteChannel(bool is_only_invite_channel) { _isOnlyInviteChannel = is_only_invite_channel; }

int Channel::getUsersLimit() const { return _usersLimit; }

void Channel::setUsersLimit(int users_limit) { _usersLimit = users_limit; }
bool Channel::isFullOfMembers() {
  return _usersLimit == -1 ? false : _members.size() >= static_cast<size_t>(_usersLimit);
}
