#include "Server.hpp"

static struct pollfd fillPollfd(int sd, short events) {
  struct pollfd fd;
  fd.fd = sd;
  fd.events = events;
  fd.revents = 0;
  return fd;
}

Server::Server(in_addr_t ip, int port, string password)
    :_socket(new Socket(AF_INET, SOCK_STREAM, 0, port, ip, 32)),
      _сontext(new Context()),
      _password(password) {
  _socket->allowReuseAddress();
  _socket->setNonblockMode();
  _socket->bindSocket();
  _socket->startListening();
  _pollfds.push_back(fillPollfd(_socket->getSockfd(), POLLIN));
}

Server::~Server() {
  delete _socket;
  delete _сontext;
}

/**
 * Start server. There is infinite loop, which handle the request
 */
void Server::start() {
  char buffer[IRC_MSG_LEN];
  ssize_t bytesRead;
  while (true) {
    polling();
    acceptNewClients();
    for (pollfdType::iterator it = _pollfds.begin() + 1, ite = _pollfds.end(); it != ite; ++it) {
      if (it->revents & POLLIN) {
        cout << "User listens:" << endl;
        bytesRead = recv(it->fd, buffer, sizeof(buffer), 0);
        if (bytesRead == 0) {
          _сontext->deleteUser(it->fd);
          close(it->fd);
          _pollfds.erase(it);
          cout << "Client ended the _userfd!" << endl;
        }
        if (bytesRead > 0) {
          buffer[bytesRead] = '\0';
          if (bytesRead - 1 >= 0 && buffer[bytesRead - 1] == '\r')
            buffer[bytesRead - 1] = '\0';
          cout << "The message was: " << buffer;
          _сontext->_handleMessage(it->fd, buffer);
        }
      }
    }
  }
}

void Server::polling() {
  int result;

  cout << "Waiting on poll()..." << endl;
  result = poll(&(_pollfds.front()), _pollfds.size(), 1000);
  if (result < 0)
    cerr << "  poll() failed: " << strerror(errno) << endl;
}

void Server::acceptNewClients() {
  int userfd;

  while (true) {
    userfd = accept(_socket->getSockfd(), nullptr, nullptr);
    if (userfd <= 0)
      break;
    _printConnectionInfo(userfd);
    _pollfds.push_back(fillPollfd(userfd, POLLIN | POLLOUT));
    _сontext->addUser(userfd);
  }
}

void Server::closeConnections(int fd) {
  size_t size = _pollfds.size();
  for (size_t i = 0; i < size; ++i) {
    if (fd == _pollfds[i].fd) {
      close(_pollfds[i].fd);
      _pollfds.erase(_pollfds.begin() + i);
      break;
    }
  }
}

bool Server::receiveRequest() { return true; }

bool Server::toSendResponse() { return true; }

/**
 * Print host and port on with user was connected
 * @param userfd
 */
void Server::_printConnectionInfo(int userfd) {
  char host[NI_MAXHOST];    // Client's remote name
  char service[NI_MAXSERV]; // Service (i.e. port) the client is connect on
  memset(host, 0, NI_MAXHOST);
  memset(service, 0, NI_MAXSERV);
  cout << "new user fd is: " << userfd;
  if (getnameinfo(reinterpret_cast<const struct sockaddr *>(&_socket->getAddress()), sizeof(sockaddr), host, NI_MAXHOST, service,
                  NI_MAXSERV, 0) != 0) {
    inet_ntop(AF_INET, &_socket->getAddress().sin_addr, host, NI_MAXHOST);
  }
  cout << " connected on host:" << host << " port: " << service << endl;
}
