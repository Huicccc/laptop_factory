#include "ClientStub.h"

#include <arpa/inet.h>
#include <iostream>

ClientStub::ClientStub() {}

int ClientStub::Init(std::string ip, int port) { return socket.Init(ip, port); }

LaptopInfo ClientStub::Order(CustomerRequest order) {
  LaptopInfo info;
  char buffer[32];
  int size;
  order.Marshal(buffer);
  size = order.Size();
  if (socket.Send(buffer, size, 0)) {
    size = info.Size();
    if (socket.Recv(buffer, size, 0)) {
      info.Unmarshal(buffer);
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
