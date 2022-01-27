#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "User.hpp"
#include "utils.hpp"
#include <string>
#include <vector>

using std::string;
using std::vector;

class User;

/**
 * Channel store password users, who owner and e.t.c.
 */
class Channel {
public:
	typedef vector<User*> usersVectorType;
	Channel(const string name);
	~Channel();

	const string& getName() const;
	const string& getPassword() const;
	void setPassword(const string password);
	const string& getTopic() const;
	void setTopic(const string& topic);
	const usersVectorType& getMembers() const;
	const usersVectorType& getOperators() const;
	const vector<string>& getBannedUsers() const;
	bool isInviteOnlyChannel() const;
	void setIsInviteOnlyChannel(bool is_only_invite_channel);
	ssize_t getUsersLimit() const;
	void setUsersLimit(int users_limit);
	void sendToAllChannelMembers(string message, User* sender = NULL);
	void addUser(User* pUser);
	/**
	 * @param pUser
	 * @return true if user added successfully or false if user already oper
	 */
	bool addOper(User* pUser);
	/**
	 * @param pUser
	 * @return true if user removed successfully or false if user wasn't found in oper array
	 */
	bool removeOper(User* pUser);
	void removeUser(User* pUser);
	int isOperator(User* pUser) const;
	bool isUserMember(User* pUser) const;
	bool isUserBanned(User* pUser) const;
	bool isPrivate() const;
	void setIsPrivate(bool is_private);
	bool isSecret() const;
	void setIsSecret(bool is_secret);
	bool isTopicSettableOnlyByOpers() const;
	void setIsTopicSettableOnlyByOpers(bool is_topic_settable_only_by_opers);
	bool isNoMessageOutside() const;
	void setIsNoMessageOutside(bool is_no_meesage_outside);
	bool isModerated() const;
	void setIsModerated(bool is_moderated);
	bool addBannerMask(const string& s);
	bool removeBannerMask(const string& s);
	bool addSpeaker(User* pUser);
	bool isSpeaker(User* pUser);
	bool removeSpeaker(User* pUser);
	bool isFullOfMembers();
	bool isBannedMask(string mask);
	User* getUserFromMembers(string userNick);

	bool isUserCanSpeak(User* pUser);

	string getUserPrefix(User* pUser);

private:
	const string _name;
	string _topic;
	string _password; // k - set a channel key (password).
	bool _isPrivate; // p
	bool _isSecret; // s
	bool _isInviteOnlyChannel; // i
	bool _isTopicSettableOnlyByOpers; // t
	bool _isNoMessageOutside; // n - no messages to channel from clients on the outside;
	bool _isModerated; // m - moderated channel;
	ssize_t _usersLimit; // l - set the user limit to channel;
	User* _owner;
	usersVectorType _members; // all participant of the channel
	usersVectorType _operators; // only operators
	usersVectorType _speakers; // v
	vector<string> _bannedUsers; // b - set a ban mask to keep users out;

	int _removeUserByNickname(usersVectorType& users, string nickname);
};

#endif
