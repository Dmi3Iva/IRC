//
// Created by Shandy Mephesto on 1/10/22.
//

#include "Server.hpp"
Server *Server::getInstance() {
  static Server instance;
  return &instance;
}

pollfd *Server::getPollFdFromFd(int fd) {
  pollfd *pollfdArr = new pollfd[1];
  pollfdArr[0].fd = fd;
  pollfdArr[0].events = POLLIN | POLLOUT;
  pollfdArr[0].revents = 0;
  return pollfdArr;
}

void Server::start() {
  _init("127.0.0.1", 6667);
  _startListening(_socketfd);

  // Grab a userfd from the queue
  pollfd *serverPollFd = getPollFdFromFd(_socketfd);

  while (!_stopFlag) {
    // accept new users
    _acceptNewClients(serverPollFd);
    _checkUsers();
  }

  delete serverPollFd;
  Users.clear();
  close(_socketfd);
  cout << "The server closed" << endl;
}

Server::~Server() {
  // Close the connection
  delete _ctx;
  close(_socketfd);
}

void Server::_init(const string addr, int port) {
  _sockaddr.sin_family = AF_INET;
  _sockaddr.sin_addr.s_addr = inet_addr(addr.c_str());
  _sockaddr.sin_port = htons(port); // htons is necessary to convert a number to network byte order
  if (bind(_socketfd, reinterpret_cast<const struct sockaddr *>(&_sockaddr), sizeof(_sockaddr)) < 0) {
    cerr << "Failed to bind to port 9999. errno: " << errno << endl;
    exit(EXIT_FAILURE);
  }
}

// Start listening. Keep as many connections as possible (SOMAXCONN)
void Server::_startListening(int socketfd) {
  if (listen(socketfd, SOMAXCONN) < 0) {
    cerr << "Failed to listen on socket. errno: " << errno << endl;
    exit(EXIT_FAILURE);
  }
}

Server::Server() : _ctx(new Context(this)) {
  _stopFlag = 0;
  _socketfd = socket(AF_INET, SOCK_STREAM, 0);

  if (_socketfd == -1) {
    cerr << "Failed to create socket. errno: " << errno << endl;
    exit(EXIT_FAILURE);
  }
  if (fcntl(_socketfd, F_SETFL, O_NONBLOCK) == -1) {
    cerr << "Failed to update socket fd. errno: " << errno << endl;
    exit(EXIT_FAILURE);
  }
}

void Server::_checkUsers() {
  for (std::vector<User>::iterator it = Users.begin(), ite = Users.end(); it != ite; ++it) {
    pollfd *userfdArr = getPollFdFromFd(it->getFD());
    // TODO:: move poll
    int ret = poll(userfdArr, 1, 800);
    cout << "User poll status " << ret << " revents: " << userfdArr[0].revents << endl;
    if (ret && (userfdArr[0].revents & POLLIN)) {
      {
        char buffer[IRC_MSG_LEN];
        ssize_t bytesRead;
        cout << "User listens:" << endl;
        if ((bytesRead = recv(it->getFD(), buffer, sizeof(buffer), 0))) {
          vector<string> commands;
          if (bytesRead == 0) {
            it->closeFD();
            Users.erase(it);
            continue;
          }
          if (bytesRead == -1) {
            cerr << "Something went wrong while try to receive message. errno: " << errno << " fd was: " << _fd << endl;
          }
          buffer[bytesRead] = '\0';
          cout << "The message was: " << buffer;
          if (it->getIsRegistered())
            _handleMessage(string(buffer));
          else
            _handleRegistration(string(buffer));
        }
        cout << "Client ended the _userfd!" << endl;
      }
    }
    delete userfdArr;
  }
}

void Server::_handleMessage(string msg) {
  // parse
  vector<string> commands = ft_split(msg, '\n');
  // send to context
  for (vector<string>::iterator it = commands.begin(), ite = commands.end(); it != ite; ++it) {
    _ctx->executeCommand(this, *it);
  }
}

void Server::_handleRegistration(User *user, string msg) {
  // parse
  vector<string> commands = ft_split(msg, '\n');
  // send to context
  for (vector<string>::iterator it = commands.begin(), ite = commands.end(); it != ite; ++it) {
    _ctx->executeCommand(this, *it);
  }
  it->setIsRegistered(true);
  string responseMessage = ":localhost 001 " + it->getNickname() + " :Welcome to the Internet Relay Network " + "!" +
                           it->getUsername() + "@" + "localhost";
  _ctx->sendMessage(it->getFd(), responseMessage);
}

void Server::_acceptNewClients(pollfd *serverPollFd) {
  int newConnectionsCount = poll(serverPollFd, 1, 500);
  cout << "New connections count: " << newConnectionsCount << endl;
  if (newConnectionsCount > 0) {
    size_t addrlen = sizeof(sockaddr);
    int userfd =
        accept(_socketfd, reinterpret_cast<struct sockaddr *>(&_sockaddr), reinterpret_cast<socklen_t *>(&addrlen));

    if (userfd < 0) {
      cerr << "Failed to accept new connection. errno: " << errno << endl;
    } else {
      fcntl(userfd, F_SETFL, O_NONBLOCK);
      Users.push_back(User(userfd));
      char host[NI_MAXHOST];    // Client's remote name
      char service[NI_MAXSERV]; // Service (i.e. port) the client is connect on
      memset(host, 0, NI_MAXHOST);
      memset(service, 0, NI_MAXSERV);
      cout << "new user fd is: " << userfd;
      if (getnameinfo(reinterpret_cast<const struct sockaddr *>(&_sockaddr), sizeof(sockaddr), host, NI_MAXHOST,
                      service, NI_MAXSERV, 0) != 0) {
        inet_ntop(AF_INET, &_sockaddr.sin_addr, host, NI_MAXHOST);
      }
      cout << " connected on host:" << host << " port: " << service << endl;
    }
  }
}
