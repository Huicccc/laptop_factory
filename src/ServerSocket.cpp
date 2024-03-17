#include <iostream> // For input and output operations
#include <string.h>
#include <arpa/inet.h> // For network operations, converting IP addresses
#include <net/if.h> // For network interface operations
#include <netdb.h> // For network database operations
#include <netinet/tcp.h> // For TCP specific constants, like TCP_NODELAY
#include <sys/types.h> // For data types used in system calls

#include "ServerSocket.h"

// Constructor that initializes a ServerSocket with a given file descriptor and Nagle algorithm setting
ServerSocket::ServerSocket(int fd, bool nagle_on) {
    fd_ = fd; // Assign the provided file descriptor to the server socket
    is_initialized_ = true; // Mark the socket as initialized
    NagleOn(nagle_on); // Set the Nagle algorithm based on the provided flag
}

// Initialize the server socket to listen on a specified port
bool ServerSocket::Init(int port) {
    if (is_initialized_) {
        return true;
    }

    struct sockaddr_in addr;
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ < 0) {
        perror("ERROR: failed to create a socket");
        return false;
    }

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; // Listen on any network interface
    addr.sin_port = htons(port); // Set the port number, converting it to network byte order

    // Bind the socket to the address and port number
    if ((bind(fd_, (struct sockaddr *) &addr, sizeof(addr))) < 0) {
        perror("ERROR: failed to bind");
        return false;
    }

    listen(fd_, 8); // Start listening on the socket with a backlog of 8 connections

    is_initialized_ = true; // Mark the socket as initialized
    return true; // Return true indicating success
}

// Accept an incoming connection and return a unique pointer to a new ServerSocket instance for the connection
std::unique_ptr<ServerSocket> ServerSocket::Accept() {
    int accepted_fd; // File descriptor for the accepted connection
    struct sockaddr_in addr; // Address of the connecting entity
    unsigned int addr_size = sizeof(addr); // Size of the address
    // Accept a connection on the socket
    accepted_fd = accept(fd_, (struct sockaddr *) &addr, &addr_size);
    if (accepted_fd < 0) {
        perror("ERROR: failed to accept connection");
        return nullptr; // Return null indicating failure to accept connection
    }

    // Create a new ServerSocket for the accepted connection and return it as a unique_ptr
    return std::unique_ptr<ServerSocket>(new ServerSocket(accepted_fd, IsNagleOn()));
}