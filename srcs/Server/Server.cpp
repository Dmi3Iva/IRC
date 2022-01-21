#include "Server.hpp"

Server::Server(string ip, string port, string password)
<<<<<<< HEAD
	: _socket(NULL)
	, _context(new Context())
	, _password(password)
{
	_address.sin_family = AF_INET;
	_address.sin_port = htons(atoi(port.c_str())); // TODO: добавить валидацию
	_address.sin_addr.s_addr = inet_addr(ip.c_str()); // TODO: добавить валидацию
}

Server::~Server()
{
	delete _socket;
	delete _context;
}

void Server::createSocket()
{
	_socket = new Socket(AF_INET, SOCK_STREAM, 0);

	_socket->setAddressReuseMode();
	_socket->setNonblockMode();
	_socket->bindToAddress(reinterpret_cast<struct sockaddr*>(&_address));
	_socket->startListening(SOMAXCONN);
=======
    :_socket(NULL),
    _context(new Context()),
      _password(password) {
  _address.sin_family = AF_INET;
  _address.sin_port = htons(atoi(port.c_str())); //TODO: добавить валидацию
  _address.sin_addr.s_addr = inet_addr(ip.c_str()); //TODO: добавить валидацию
}

Server::~Server() {
  delete _socket;
  delete _context;
}

void Server::createSocket() {
  _socket = new Socket(AF_INET, SOCK_STREAM, 0);

  _socket->setAddressReuseMode();
  _socket->setNonblockMode();
  _socket->bindToAddress(reinterpret_cast<struct sockaddr *>(&_address));
  _socket->startListening(SOMAXCONN);
>>>>>>> 47d11f854bbd249013107e56f971bae3c3de3713
}

/**
 * Start server. There is infinite loop, which handle the request
 */
<<<<<<< HEAD
void Server::start()
{
	createSocket();
	_pollfds.push_back(fillPollfd(_socket->getSockfd(), POLLIN));

	while (true) {
		polling();
		for (pollfdType::iterator it = _pollfds.begin(), ite = _pollfds.end(); it != ite; ++it) {
			if (it->revents == 0)
				continue;
			if (it->revents) {
				if (it->fd == _socket->getSockfd())
					acceptNewClients();
				else if (!receiveMessage(_context->findUserByFd(it->fd))) {
					_pollfds.erase(it);
					break;
				}
			}
		}
	}
}

void Server::polling()
{
	if (poll(&(_pollfds.front()), _pollfds.size(), POLL_TIMEOUT) < 0)
		cerr << "  poll() failed: " << strerror(errno) << endl;
}

void Server::acceptNewClients()
{
	int userfd;

	while (true) {
		userfd = accept(_socket->getSockfd(), NULL, NULL);
		if (userfd <= 0)
			break;
		pair<string, string> info = _getConnectionInfo(userfd);
		_pollfds.push_back(fillPollfd(userfd, POLLIN));
		_context->addUser(new User(userfd, info.first, info.second));
	}
}

bool Server::receiveMessage(User* user)
{
	char buffer[IRC_MSG_LEN];
	ssize_t bytesRead;

	cout << "User listens:" << endl;
	bytesRead = recv(user->getFD(), buffer, sizeof(buffer), 0);

	if (bytesRead == 0) {
		cout << "Client ended the _userfd!" << user->getFD() << endl;
		close(user->getFD());
		_context->deleteUser(user);
		return false;
	}

	if (bytesRead > 0) {
		buffer[bytesRead] = '\0';
		cout << "The message was: " << buffer;
		user->appendMessage(string(buffer, bytesRead));
		_context->handleMessage(user); // TODO: вынести отсюда
	}

	return true;
=======
void Server::start() {
  createSocket();
  _pollfds.push_back(fillPollfd(_socket->getSockfd(), POLLIN));

  while (true) {
    polling();
    for (pollfdType::iterator it = _pollfds.begin(), ite = _pollfds.end(); it != ite; ++it) {
      if (it->revents == 0)
        continue;
      if (it->revents) {
        if (it->fd == _socket->getSockfd())
          acceptNewClients();
        else if (!receiveMessage(_context->findUserByFd(it->fd))) {
          _pollfds.erase(it);
          break;
        }
      }
    }
  }
}

void Server::polling() {
  if (poll(&(_pollfds.front()), _pollfds.size(), POLL_TIMEOUT) < 0)
    cerr << "  poll() failed: " << strerror(errno) << endl;
}

void Server::acceptNewClients() {
  int userfd;

  while (true) {
    userfd = accept(_socket->getSockfd(), NULL, NULL);
    if (userfd <= 0)
      break;
    _printConnectionInfo(userfd);
    _pollfds.push_back(fillPollfd(userfd, POLLIN));
    _context->addUser(new User(userfd));
  }
}

bool Server::receiveMessage(User* user) {
  char buffer[IRC_MSG_LEN];
  ssize_t bytesRead;

  cout << "User listens:" << endl;
  bytesRead = recv(user->getFD(), buffer, sizeof(buffer), 0);

  if (bytesRead == 0) {
    cout << "Client ended the _userfd!" << user->getFD() << endl;
    close(user->getFD());
    _context->deleteUser(user);
    return false;
  }

  if (bytesRead > 0) {
    buffer[bytesRead] = '\0';
    cout << "The message was: " << buffer;
    user->appendMessage(string(buffer, bytesRead));
    _context->_handleMessage(user); // TODO: вынести отсюда
  }

  return true;
>>>>>>> 47d11f854bbd249013107e56f971bae3c3de3713
}

/**
 * Print host and port on with user was connected
 * @param userfd
 */
<<<<<<< HEAD
pair<string, string> Server::_getConnectionInfo(int userfd)
{
	char host[NI_MAXHOST]; // Client's remote name
	char service[NI_MAXSERV]; // Service (i.e. port) the client is connect on
	memset(host, 0, NI_MAXHOST);
	memset(service, 0, NI_MAXSERV);
	cout << "new user fd is: " << userfd;
	if (getnameinfo(reinterpret_cast<const struct sockaddr*>(&_address), sizeof(sockaddr), host, NI_MAXHOST, service,
			NI_MAXSERV, 0)
		!= 0) {
		inet_ntop(AF_INET, &_address.sin_addr, host, NI_MAXHOST);
	}
	cout << " connected on host:" << host << " port: " << service << endl;
	return make_pair(string(host), string(service));
=======
void Server::_printConnectionInfo(int userfd) {
  char host[NI_MAXHOST];    // Client's remote name
  char service[NI_MAXSERV]; // Service (i.e. port) the client is connect on
  memset(host, 0, NI_MAXHOST);
  memset(service, 0, NI_MAXSERV);
  cout << "new user fd is: " << userfd;
  if (getnameinfo(reinterpret_cast<const struct sockaddr *>(&_address), sizeof(sockaddr), host, NI_MAXHOST, service,
                  NI_MAXSERV, 0) != 0) {
    inet_ntop(AF_INET, &_address.sin_addr, host, NI_MAXHOST);
  }
  cout << " connected on host:" << host << " port: " << service << endl;
>>>>>>> 47d11f854bbd249013107e56f971bae3c3de3713
}
