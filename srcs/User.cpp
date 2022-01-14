//
// Created by Shandy Mephesto on 1/10/22.
//

#include "User.hpp"

User::User(int userFd) : _fd(userFd), _isRegistered(false) {}
User::~User() {}

// int User::listen() {
//   // Read from the _userfd
// }

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
