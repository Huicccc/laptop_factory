#include "ClientStub.h"

#include <arpa/inet.h>
#include <iostream>

ClientStub::ClientStub() {}

//  initializes the client stub and establishes a new TCP connection to the server
int ClientStub::Init(std::string ip, int port) {
    return socket.Init(ip, port);
}

//  sends an order to the server and receives the laptop (or laptop information) from the server.
// 1. Once the function is called the order details should be marshalled into a byte stream.
// 2. The byte stream is then sent through a socket connection to the server.
// 3. Next, the function should wait for a server response.
// 4. Once the server responds back with the laptop information in a byte stream format, the byte stream should be unmarshalled to the laptop information and returned.
// LaptopInfo ClientStub::OrderLaptop(LaptopOrder order) {
LaptopInfo ClientStub::Order(CustomerRequest order) {    
    LaptopInfo info;
    char buffer[32];
    order.Marshal(buffer); // 1. Once the function is called the order details should be marshalled into a byte stream.
    if (socket.Send(buffer, order.Size(), 0)) { // 2. The byte stream is then sent through a socket connection to the server.
        if (socket.Recv(buffer, info.Size(), 0)) { // 3. Next, the function should wait for a server response.
            info.Unmarshal(buffer); // 4. Once the server responds back with the laptop information in a byte stream format, the byte stream should be unmarshalled to the laptop information and returned.
        }
    }
    return info;
}

CustomerRecord ClientStub::ReadRecord(CustomerRequest order) {
  CustomerRecord record;
  char buffer[32];
  int size;
  order.Marshal(buffer);
  size = order.Size();
  if (socket.Send(buffer, size, 0)) {
    size = record.Size();
    if (socket.Recv(buffer, size, 0)) {
      record.Unmarshal(buffer);
    }
  }
  return record;
}

LogResponse ClientStub::BackupRecord(LogRequest log) {
  LogResponse resp;
  char buffer[32];
  int size;
  log.Marshal(buffer);
  size = log.Size();
  if (socket.Send(buffer, size, 0)) {
    size = resp.Size();
    if (socket.Recv(buffer, size, 0)) {
      resp.Unmarshal(buffer);
    }
  }
  return resp;
}

void ClientStub::Identify(int role) {
  int net_role = htonl(role);
  socket.Send((char *)&net_role, sizeof(net_role), 0);
}