#include "Server.hpp"

Server::Server() : _ctx(new Context()) {
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

/**
 * It is singleton
 * @return single instance of server
 */
Server *Server::getInstance() {
  static Server instance;
  return &instance;
}

Server::~Server() {
  delete _ctx;
  close(_socketfd);
}

/**
 * Start server. There is infinite loop, which handle the request
 */
void Server::start() {
  _bindSocket("127.0.0.1", 6667);
  _startListening(_socketfd);

  pollfd *serverPollFd = getPollFdFromFd(_socketfd);

  // TODO:: it's not good to have infinite loop here
  while (true) {
    // accept new users
    _acceptNewClients(serverPollFd);
    // if poll show new messages then check users in context
    _ctx->listenUsers();
  }

  //  delete serverPollFd;
  //  close(_socketfd);
  //  cout << "The server closed" << endl;
}

/**
 * Bind socket on some address with port
 * @param addr
 * @param port
 */
void Server::_bindSocket(string addr, int port) {
  _sockaddr.sin_family = AF_INET;
  _sockaddr.sin_addr.s_addr = inet_addr(addr.c_str());
  _sockaddr.sin_port = htons(port); // htons is necessary to convert a number to network byte order
  if (bind(_socketfd, reinterpret_cast<const struct sockaddr *>(&_sockaddr), sizeof(_sockaddr)) < 0) {
    cerr << "Failed to bind to port 9999. errno: " << errno << endl;
    exit(EXIT_FAILURE);
  }
}

/**
 * Start listening. Keep as many connections as possible (SOMAXCONN)
 * @param socketfd
 */
void Server::_startListening(int socketfd) {
  if (listen(socketfd, SOMAXCONN) < 0) {
    cerr << "Failed to listen on socket. errno: " << errno << endl;
    exit(EXIT_FAILURE);
  }
}

/**
 * Accept new clients
 * @param serverPollFd
 */
void Server::_acceptNewClients(pollfd *serverPollFd) {
  int newConnectionsCount = poll(serverPollFd, 1, 1000);
  cout << "New connections count: " << newConnectionsCount << endl;
  if (newConnectionsCount > 0) {
    size_t addrlen = sizeof(sockaddr);
    int userfd =
        accept(_socketfd, reinterpret_cast<struct sockaddr *>(&_sockaddr), reinterpret_cast<socklen_t *>(&addrlen));

    if (userfd < 0) {
      cerr << "Failed to accept new connection. errno: " << errno << endl;
    } else {
      fcntl(userfd, F_SETFL, O_NONBLOCK);
      _printConnectionInfo(userfd);
      // add connection to context
      _ctx->addUser(userfd);
    }
  }
}

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
  if (getnameinfo(reinterpret_cast<const struct sockaddr *>(&_sockaddr), sizeof(sockaddr), host, NI_MAXHOST, service,
                  NI_MAXSERV, 0) != 0) {
    inet_ntop(AF_INET, &_sockaddr.sin_addr, host, NI_MAXHOST);
  }
  cout << " connected on host:" << host << " port: " << service << endl;
}
