#include <arpa/inet.h>
#include <cerrno>
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h> // For sockaddr_in
#include <poll.h>
#include <sys/socket.h> // For socket functions
#include <unistd.h>

using std::cerr;
using std::cout;
using std::endl;
using std::string;

int main(void) {
  // Create a socket (IPv4, TCP)
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    cerr << "Failed to create socket. errno: " << errno << endl;
    exit(EXIT_FAILURE);
  }
  cout << "Socket created succesfully!" << endl;

  // Listen to port 9999 on any address
  sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  sockaddr.sin_port = htons(9999); // htons is necessary to convert a number to network byte order
  if (bind(sockfd, reinterpret_cast<const struct sockaddr *>(&sockaddr), sizeof(sockaddr)) < 0) {
    cerr << "Failed to bind to port 9999. errno: " << errno << endl;
    exit(EXIT_FAILURE);
  }
  cout << "Bound to port successfully!" << endl;

  // TODO:: there is suggestion to use poll
  // Start listening. Keep as many connections as possible (SOMAXCONN)
  if (listen(sockfd, SOMAXCONN) < 0) {
    cerr << "Failed to listen on socket. errno: " << errno << endl;
    exit(EXIT_FAILURE);
  }

  // Grab a connection from the queue
  size_t addrlen = sizeof(sockaddr);
  int connection =
      accept(sockfd, reinterpret_cast<struct sockaddr *>(&sockaddr), reinterpret_cast<socklen_t *>(&addrlen));
  if (connection < 0) {
    cerr << "Failed to grab connection. errno: " << errno << endl;
    exit(EXIT_FAILURE);
  } else {
    char host[NI_MAXHOST];       // Client's remote name
    char service[NI_MAXSERV];    // Service (i.e. port) the client is connect on
    memset(host, 0, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);

    if (getnameinfo(reinterpret_cast<const struct sockaddr *>(&sockaddr), sizeof(sockaddr), host, NI_MAXHOST, service,
                    NI_MAXSERV, 0) == 0) {
      cout << host << " connected on port " << service << endl;
    } else {
      inet_ntop(AF_INET, &sockaddr.sin_addr, host, NI_MAXHOST);
      cout << host << " connected on port " << ntohs(sockaddr.sin_port) << endl;
    }
  }

  // Read from the connection
  char buffer[100];
  ssize_t bytesRead;
  while ((bytesRead = recv(connection, buffer, 100, 0))) {
    if (bytesRead == -1) {
      cerr << "Something went wrong while try to receive message. errno: " << errno << endl;
      close(connection);
      close(sockfd);
      exit(EXIT_FAILURE);
    }
    buffer[bytesRead] = '\0';
    cout << "The message was: " << buffer;
    if (bytesRead == 1 && buffer[0] == '#') {
      cout << "end signal from the client" << endl;
      close(connection);
      close(sockfd);
      exit(EXIT_FAILURE);
    }
    // Send a message to the connection
    send(connection, buffer, bytesRead + 1, 0);
  }

  // Close the connections
  close(connection);
  close(sockfd);
}