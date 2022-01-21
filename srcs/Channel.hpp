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
  typedef vector<User *> usersVectorType;
  Channel(const string name);
  ~Channel();

  const string &getName() const;
  const string &getPassword() const;
  void setPassword(const string password);
  const string &getTopic() const;
  void setTopic(const string &topic);
  const usersVectorType &getMembers() const;
  const usersVectorType &getOperators() const;
  const vector<string> &getBannedUsers() const;
  bool isOnlyInviteChannel() const;
  void setIsOnlyInviteChannel(bool is_only_invite_channel);
  ssize_t getUsersLimit() const;
  void setUsersLimit(int users_limit);
  void sendToAllChannelMembers(string message, User *sender = NULL);

  void addUser(User *pUser);
  void removeUser(User *pUser);
  int isOperator(User *pUser) const;
  bool isUserMember(User *pUser) const;
  bool isUserBanned(User *pUser) const;

  bool isFullOfMembers();

private:
  const string _name;
  string _topic;
  string _password;
  bool _isOnlyInviteChannel;
  User *_owner;
  ssize_t _usersLimit;

  // all participant of the channel
  usersVectorType _members;
  // only operators
  usersVectorType _operators;
  vector<string> _bannedUsers;

  int _removeUserByNickname(usersVectorType &users, string nickname);
};

#endif
