#include "Socket.h"

#include <iostream>

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <sys/ioctl.h>
#include <sys/types.h>

Socket::Socket() : is_initialized_(false), nagle_(NAGLE_ON) {}

Socket::~Socket() {
    if (is_initialized_) {
        Close();
    }
}

void Socket::Close() {
    shutdown(fd_, SHUT_RDWR);
    close(fd_);
    is_initialized_ = false;
}

int Socket::Send(char *buffer, int size, int flags) {
    int bytes_written = 0;
    int offset = 0; // Keeps track of how much data has been sent
    while (size > 0) {
        bytes_written = send(fd_, buffer + offset, size, flags); // Attempt to send data
        if (bytes_written < 0) {
            // If send fails, report error and close the socket
            perror("ERROR: failed to send");
            Close(); // Ensure the socket is closed on error
            return 0; // Indicate failure
        }
        size -= bytes_written; // Decrease remaining size
        offset += bytes_written; // Increase offset
        assert(size >= 0); // Ensure we haven't underflowed size
    }
    return 1; // Indicate success
}

int Socket::Recv(char *buffer, int size, int flags) {
    int bytes_read = 0;
    int offset = 0;
    while (size > 0) {
        bytes_read = recv(fd_, buffer + offset, size, flags);
        if (bytes_read <= 0) {
            Close();
            return 0;
        }
        assert(bytes_read != 0);
        size -= bytes_read;
        offset += bytes_read;
        assert(size >= 0);
    }
    return 1;
}

int Socket::NagleOn(bool on_off) {
    nagle_ = (on_off ? NAGLE_ON : NAGLE_OFF);
    int result = setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, (void *) &nagle_, sizeof(int));
    if (result < 0) {
        perror("ERROR: setsockopt failed");
        return 0;
    }
    return 1;
}

bool Socket::IsNagleOn() {
    return (nagle_ == NAGLE_ON);
}