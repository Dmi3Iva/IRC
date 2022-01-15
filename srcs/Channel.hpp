#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "User.hpp"
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
  void addUser(User *p_user);
  const usersVectorType &getMembers() const;
  const usersVectorType &getOperators() const;
  const usersVectorType &getBannedUsers() const;

  void removeUser(User *pUser);
  int isOperator(User *pUser) const;

  bool isOnlyInviteChannel() const;
  void setIsOnlyInviteChannel(bool is_only_invite_channel);
  int getUsersLimit() const;
  void setUsersLimit(int users_limit);

  bool isFullOfMembers();

private:
  const string _name;
  string _topic;
  string _password;
  bool _isOnlyInviteChannel;
  User *_owner;
  // all participant of the channel
  usersVectorType _members;
  // only operators
  usersVectorType _operators;
  usersVectorType _bannedUsers;
  ssize_t _usersLimit;
  int _removeUserByNickname(usersVectorType &users, string nickname);
};

#endif
