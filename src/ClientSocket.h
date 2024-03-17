#ifndef __CLIENTSOCKET_H__
#define __CLIENTSOCKET_H__

#include <string>

#include "Socket.h"

class ClientSocket : public Socket {
public:
    ClientSocket() {}
    ~ClientSocket() {}

    // Declare the Init function which initializes the socket connection
    // Takes an IP address as a std::string and a port number as an int
    // Returns an int to indicate success or failure
    int Init(std::string ip, int port);
};

#endif // __CLIENTSOCKET_H__ End
