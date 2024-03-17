#include <string.h> // Provides declarations for string handling functions like memset, strcpy, etc.
#include <arpa/inet.h> // Defines operations for converting IP addresses from text to binary form and vice versa, such as inet_addr and inet_ntoa
#include <net/if.h> // Defines constants and structures needed for network interface operations, like struct ifreq used in ioctl calls
#include <netdb.h>
#include <netinet/tcp.h>
#include <sys/types.h>

#include "ClientSocket.h"

int ClientSocket::Init(std::string ip, int port) {
    if (is_initialized_) {
        return 0;
    }

    // Declare sockaddr_in structure for the server's address and create the socket
    struct sockaddr_in addr;
    fd_ = socket(AF_INET, SOCK_STREAM, 0); // Create a socket for IPv4 and TCP
    if (fd_ < 0) {
        perror("ERROR: failed to create a socket"); // Print error if socket creation fails
        return 0; // Return 0 to indicate failure
    }

    // Initialize the address structure
    memset(&addr, '\0', sizeof(addr)); // Clear structure
    addr.sin_family = AF_INET; // Set the address family to IPv4
    addr.sin_addr.s_addr = inet_addr(ip.c_str()); // Convert IP address from string to binary
    addr.sin_port = htons(port); // Convert port number to network byte order

    // Attempt to connect to the server with the specified address and port
    if ((connect(fd_, (struct sockaddr *) &addr, sizeof(addr))) < 0) {
        perror("ERROR: failed to connect"); // Print error if connection fails
        return 0; // Return 0 to indicate failure
    }

    // Mark the socket as initialized upon successful connection
    is_initialized_ = true;
    return 1; // Return 1 to indicate success
}