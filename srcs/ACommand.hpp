#ifndef ACOMMAND
#define ACOMMAND

#include "Context.hpp"
#include "User.hpp"
#include <string>

using std::string;

class Context;
class User;

class ACommand {
public:
  ACommand(Context *ctx);
  ~ACommand(){};
  virtual void execute(User *user, string cmd) = 0;
  //  virtual void response(string args) = 0;

protected:
  string _name;
  Context *_ctx;
};
#endif