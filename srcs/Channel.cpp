#include "Channel.hpp"

Channel::Channel(const string name)
	: _name(name)
	, _topic("default topic: welcome to the channel! :)")
	, _isPrivate(false)
	, _isSecret(false)
	, _isInviteOnlyChannel(false)
	, _isTopicSettableOnlyByOpers(false)
	, _isNoMessageOutside(true)
	, _isModerated(false)
	, _usersLimit(-1)
	, _owner(NULL)
{
}

Channel::~Channel()
{
	_members.clear();
	_operators.clear();
	_bannedUsers.clear();
	cout << "Channel " << _name << " is closed" << endl;
}

const string& Channel::getName() const { return _name; }

const string& Channel::getPassword() const { return _password; }

void Channel::setPassword(const string password) { _password = password; }

void Channel::addUser(User* pUser)
{
	// if user already in the channel don't add him/her
	if (!isUserMember(pUser)) {
		_members.push_back(pUser);
		if (!_owner) {
			_owner = pUser;
			_operators.push_back(pUser);
		}
	}
}
const string& Channel::getTopic() const { return _topic; }

void Channel::setTopic(const string& topic) { _topic = topic; }

const Channel::usersVectorType& Channel::getMembers() const { return _members; }

const Channel::usersVectorType& Channel::getOperators() const { return _operators; }

void Channel::removeUser(User* pUser)
{
	// remove user from _members by nick
	_removeUserByNickname(_members, pUser->getNickname());
	// remove user from _operators by nick
	_removeUserByNickname(_operators, pUser->getNickname());
	// if was removed owner, replace owner with new member
	if (!_members.empty() && pUser->getNickname() == _owner->getNickname()) {
		if (!_operators.empty()) {
			_owner = _operators.front();
		} else {
			_owner = _members.front();
			_operators.push_back(_owner);
		}
	}
}

int Channel::_removeUserByNickname(Channel::usersVectorType& users, string nickname)
{
	for (usersVectorType::iterator it = users.begin(), ite = users.end(); it != ite; ++it) {
		if ((*it)->getNickname() == nickname) {
			users.erase(it);
			return 1;
		}
	}
	return 0;
}

int Channel::isOperator(User* pUser) const
{
	for (usersVectorType::const_iterator it = _operators.begin(), ite = _operators.end(); it != ite; ++it) {
		if ((*it)->getNickname() == pUser->getNickname())
			return 1;
	}
	return 0;
}
const vector<string>& Channel::getBannedUsers() const { return _bannedUsers; }

bool Channel::isInviteOnlyChannel() const { return _isInviteOnlyChannel; }

void Channel::setIsInviteOnlyChannel(bool is_only_invite_channel) { _isInviteOnlyChannel = is_only_invite_channel; }

ssize_t Channel::getUsersLimit() const { return _usersLimit; }

void Channel::setUsersLimit(int users_limit) { _usersLimit = users_limit; }

bool Channel::isFullOfMembers() { return _usersLimit != -1 && _members.size() >= static_cast<size_t>(_usersLimit); }

void Channel::sendToAllChannelMembers(string message, User* sender)
{
	for (usersVectorType::iterator user = _members.begin(); user != _members.end(); user++) {
		if (!sender || sender->getNickname() != (*user)->getNickname())
			sendMessage((*user)->getFD(), message);
	}
}

bool Channel::isUserMember(User* pUser) const
{
	for (usersVectorType ::const_iterator it = _members.begin(), ite = _members.end(); it != ite; ++it) {
		if ((*it)->getNickname() == pUser->getNickname()) {
			return true;
		}
	}
	return false;
}

User* Channel::getUserFromMembers(string userNick)
{
	for (usersVectorType ::const_iterator it = _members.begin(), ite = _members.end(); it != ite; ++it) {
		if ((*it)->getNickname() == userNick) {
			return *it;
		}
	}
	return NULL;
}

bool Channel::isUserBanned(User* pUser) const
{
	for (vector<string>::const_iterator it = _bannedUsers.begin(), ite = _bannedUsers.end(); it != ite; ++it) {
		if (*it == pUser->getNickname()) {
			return true;
		}
	}
	return false;
}

bool Channel::addOper(User* pUser)
{
	if (isOperator(pUser))
		return false;
	_operators.push_back(pUser);
	return true;
}

bool Channel::removeOper(User* pUser)
{
	for (usersVectorType::const_iterator it = _operators.begin(), ite = _operators.end(); it != ite; ++it) {
		if ((*it)->getNickname() == pUser->getNickname()) {
			_operators.erase(it);
			return true;
		}
	}
	return false;
}
bool Channel::isPrivate() const { return _isPrivate; }
void Channel::setIsPrivate(bool is_private) { _isPrivate = is_private; }
bool Channel::isSecret() const { return _isSecret; }
void Channel::setIsSecret(bool is_secret) { _isSecret = is_secret; }
bool Channel::isTopicSettableOnlyByOpers() const { return _isTopicSettableOnlyByOpers; }
void Channel::setIsTopicSettableOnlyByOpers(bool is_topic_settable_only_by_opers) { _isTopicSettableOnlyByOpers = is_topic_settable_only_by_opers; }
bool Channel::isNoMessageOutside() const { return _isNoMessageOutside; }
void Channel::setIsNoMessageOutside(bool is_no_meesage_outside) { _isNoMessageOutside = is_no_meesage_outside; }
bool Channel::isModerated() const { return _isModerated; }
void Channel::setIsModerated(bool is_moderated) { _isModerated = is_moderated; }

bool Channel::addBannerMask(const string& mask)
{
	if (std::find(_bannedUsers.begin(), _bannedUsers.end(), mask) == _bannedUsers.end()) {
		_bannedUsers.push_back(mask);
		return true;
	}
	return false;
}

bool Channel::removeBannerMask(const string& mask)
{
	vector<string>::iterator it = std::find(_bannedUsers.begin(), _bannedUsers.end(), mask);
	if (it != _bannedUsers.end()) {
		_bannedUsers.erase(it);
		return true;
	}
	return false;
}

bool Channel::addSpeaker(User* pUser)
{
	for (usersVectorType::const_iterator it = _speakers.begin(), ite = _speakers.end(); it != ite; ++it) {
		if ((*it)->getNickname() == pUser->getNickname())
			return false;
	}
	_speakers.push_back(pUser);
	return true;
}

bool Channel::removeSpeaker(User* pUser)
{
	for (usersVectorType::const_iterator it = _speakers.begin(), ite = _speakers.end(); it != ite; ++it) {
		if ((*it)->getNickname() == pUser->getNickname()) {
			_speakers.erase(it);
			return true;
		}
	}
	return false;
}

bool Channel::isBanned(string userName)
{
	for (vector<string>::iterator it = _bannedUsers.begin(); it != _bannedUsers.end(); ++it) {
		if (isMaskMatch(userName, *it))
			return true;
	}
	return false;
}
bool Channel::isBanned(User* user) { return isBanned(user->getNickname() + "!" + user->getUsername() + "@" + user->getHostname()); }

bool Channel::isSpeaker(User* pUser)
{
	for (usersVectorType ::const_iterator it = _speakers.begin(); it != _speakers.end(); ++it) {
		if ((*it)->getNickname() == pUser->getNickname()) {
			return true;
		}
	}
	return false;
}

/**
 * In common case: if *outside messages allowed and user not banned in the channel) or user is member then user can speak
 * In case of moderated Channel user can speak only if he/she is an operator or speaker
 * @param pUser
 * @return
 */
bool Channel::isUserCanSpeak(User* pUser)
{
	return _isModerated //
		? isOperator(pUser) || isSpeaker(pUser) //
		: (!_isNoMessageOutside && !isBanned(pUser)) || isUserMember(pUser);
}

string Channel::getUserPrefix(User* pUser)
{
	return isOperator(pUser) ? "@" // for oper
		: isSpeaker(pUser)	 ? "+" // for speaker
							 : ""; // just member
}
