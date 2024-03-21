#include "ServerStub.h"

ServerStub::ServerStub() {}

// initializes the server stub. It should directly take a socket or a class that includes a socket. 
// In either case, the socket should be the one “accepted” from a listening socket and should be already connected to a client.
void ServerStub::Init(std::unique_ptr <ServerSocket> socket) {
    this->socket = std::move(socket);
}


// receives an order from the client.
// 1. The function waits for the client order to arrive through the socket connection.
// 2. Once the order is received through the socket in a byte stream format, the byte stream should be unmarshalled to an order and should be returned.
// LaptopOrder ServerStub::ReceiveOrder() {
//     char buffer[32];
//     LaptopOrder order;
//     if (socket->Recv(buffer, order.Size(), 0)) {
//         order.Unmarshal(buffer);
//     }
//     return order;
// }
// The order information will be used to assemble the laptop (i.e., filling in laptop information; see later sections). 
// Once the laptop assembly is finished, the laptop information should be sent back to the customer.
CustomerRequest ServerStub::ReceiveRequest() {
  char buffer[32];
  CustomerRequest request;
  if (socket->Recv(buffer, request.Size(), 0)) {
    request.Unmarshal(buffer);
  }
  return request;
}

// send the laptop information to the client who ordered it. 
// It should marshal the laptop information into a byte stream, and send the byte stream through the connected socket to the customer.
int ServerStub::SendLaptop(LaptopInfo info) {
    char buffer[32];
    info.Marshal(buffer);
    return socket->Send(buffer, info.Size(), 0);
}


int ServerStub::ReturnRecord(CustomerRecord record) {
  char buffer[32];
  record.Marshal(buffer);
  return socket->Send(buffer, record.Size(), 0);
}

int ServerStub::ReceiveIndentity() {
  char buffer[32];
  int identity = -1;
  if (socket->Recv(buffer, sizeof(int), 0)) {
    identity = ntohl(*(int *)buffer);
  }
  return identity;
}

LogRequest ServerStub::ReceiveLogRequest() {
  char buffer[32];
  LogRequest response;
  if (socket->Recv(buffer, response.Size(), 0)) {
    response.Unmarshal(buffer);
  }
  return response;
}

int ServerStub::ReturnLogResponse(LogResponse response) {
  char buffer[32];
  response.Marshal(buffer);
  return socket->Send(buffer, response.Size(), 0);
}