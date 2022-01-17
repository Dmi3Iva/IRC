#include "Server.hpp"

static struct pollfd fillPollfd(int sd, short events) {
  struct pollfd fd;
  fd.fd = sd;
  fd.events = events;
  fd.revents = 0;
  return fd;
}

Server::Server(in_addr_t ip, int port, string password)
    :_socket(new Socket(AF_INET, SOCK_STREAM, 0, port, ip, 32)), _сontext(new Context()),
    _password(password) {
  _socket->allowReuseAddress();
  _socket->setNonblockMode();
  _socket->bindSocket();
  _socket->startListening();
  _pollfds.push_back(fillPollfd(_socket->getSockfd(), POLLIN));

//  _socketfd = socket(AF_INET, SOCK_STREAM, 0);
//
//  if (_socketfd == -1) {
//    cerr << "Failed to create socket. errno: " << errno << endl;
//    exit(EXIT_FAILURE);
//  }
//  int on = 1;
//  setsockopt(_socketfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
//  if (fcntl(_socketfd, F_SETFL, O_NONBLOCK) == -1) {
//    cerr << "Failed to update socket fd. errno: " << errno << endl;
//    exit(EXIT_FAILURE);
//  }
}

///**
// * It is singleton
// * @return single instance of server
// */
//Server *Server::getInstance() {
//  static Server instance;
//  return &instance;
//}

Server::~Server() {
  delete _socket;
  delete _сontext;
//  close(_socketfd);
}

/**
 * Start server. There is infinite loop, which handle the request
 */
void Server::start() {
  while (true) {
    polling();
    acceptNewClients();
    _сontext->listenUsers();
  }
//  _bindSocket("127.0.0.1", 6667);
//  _startListening(_socketfd);

//  pollfd *serverPollFd = getPollFdFromFd(_socketfd);
//
//  // TODO:: it's not good to have infinite loop here
//  while (true) {
//    // accept new users
//    _acceptNewClients(serverPollFd);
//    // if poll show new messages then check users in context
//    _сontext->listenUsers();
//  }

  //  delete serverPollFd;
  //  close(_socketfd);
  //  cout << "The server closed" << endl;
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

///**
// * Bind socket on some address with port
// * @param addr
// * @param port
// */
//void Server::_bindSocket(string addr, int port) {
//  _sockaddr.sin_family = AF_INET;
//  _sockaddr.sin_addr.s_addr = inet_addr(addr.c_str());
//  _sockaddr.sin_port = htons(port); // htons is necessary to convert a number to network byte order
//  if (bind(_socketfd, reinterpret_cast<const struct sockaddr *>(&_sockaddr), sizeof(_sockaddr)) < 0) {
//    cerr << "Failed to bind to port 9999. errno: " << errno << endl;
//    exit(EXIT_FAILURE);
//  }
//}

///**
// * Start listening. Keep as many connections as possible (SOMAXCONN)
// * @param socketfd
// */
//void Server::_startListening(int socketfd) {
//  if (listen(socketfd, SOMAXCONN) < 0) {
//    cerr << "Failed to listen on socket. errno: " << errno << endl;
//    exit(EXIT_FAILURE);
//  }
//}

/**
 * Accept new clients
 * @param serverPollFd
 */
//void Server::_acceptNewClients(pollfd *serverPollFd) {
//  int newConnectionsCount = poll(serverPollFd, 1, 1000);
//  cout << "New connections count: " << newConnectionsCount << endl;
//  if (newConnectionsCount > 0) {
//    size_t addrlen = sizeof(sockaddr);
//    int userfd =
//        accept(_socketfd, reinterpret_cast<struct sockaddr *>(&_sockaddr), reinterpret_cast<socklen_t *>(&addrlen));
//
//    if (userfd < 0) {
//      cerr << "Failed to accept new connection. errno: " << errno << endl;
//    } else {
//      fcntl(userfd, F_SETFL, O_NONBLOCK);
//      _printConnectionInfo(userfd);
//      // add connection to context
//      _сontext->addUser(userfd);
//    }
//  }
//}

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
