#include "ClientStub.h"

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
LaptopInfo ClientStub::OrderLaptop(LaptopOrder order) {
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