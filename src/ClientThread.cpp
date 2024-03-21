#include "ClientThread.h"
#include "Messages.h"

#include <iostream>

/*
    You will model each customer as a thread and your client program will model multiple customers that concurrently issue orders to you factory. 
    Your client program should take command line arguments that specify the ip address of the server, the port number of the server, the number of customers, how many orders each customer will place, and the laptop type that the customers want. 
    The client program should take these as arguments in the following format:
    ./client [ip addr] [port #] [# customers] [# orders] [laptop type]
    For example,
    ./client 123.456.789.123 12345 16 1000 0
    means that the server ip address is 123.456.789.123, the server port number is 12345, the number of customers is 16, 
    and each customer will place 1000 orders, where the laptop type is regular.

    When the client program starts up,
    1. The program should create the customer threads as many as the specified customer number.
    2. Each customer thread should have a unique customer id.
    3. Either the main thread or each customer thread can instantiate connection to the server, but the socket connection should be made once per client stub and each customer should have its own client stub instance.
    4. The customer thread should start issuing orders and receiving laptop information as many times as the input argument using the client stub described in the previous section. The order should include the corresponding customer id, order number, and laptop type. For now, only use laptop type 0 (regular type).
    5. Once the thread completed all of its tasks, the connection to the server should be closed and the thread should terminate.
    By varying the number of customers you should be able to control the amount of concurrent loads on the server.
*/
ClientThreadClass::ClientThreadClass() {}

void ClientThreadClass::
ThreadBody(std::string ip, int port, int id, int orders, int type) {
    customer_id = id; // 2. Each customer thread should have a unique customer id.
    num_orders = orders; 
    // laptop_type = type; 
    request_type = type;

    // 3. Either the main thread or each customer thread can instantiate connection to the server
    // but the socket connection should be made once per client stub and each customer should have its own client stub instance.
    if (!stub.Init(ip, port)) { // ClientStub stub;
        std::cout << "Thread " << customer_id << " failed to connect" << std::endl;
        return; // Exit if connection to the server fails
    }

    stub.Identify(0);
    if (request_type == 1) {
        Orders();
    } else if (request_type == 2) {
        Records();
    } else if (request_type == 3) {
        ScanRecords();
    }
}

ClientTimer ClientThreadClass::GetTimer() {
    return timer;
}

void ClientThreadClass::Orders() {
  for (int i = 0; i < num_orders; i++) {
    CustomerRequest request;
    LaptopInfo laptop;
    request.SetOrder(customer_id, i, request_type);

    timer.Start();
    laptop = stub.Order(request);
    timer.EndAndMerge();

    if (!laptop.IsValid()) {
      std::cout << "Invalid laptop " << customer_id << std::endl;
      break;
    }
  }
}

void ClientThreadClass::Records() {
  CustomerRequest request;
  CustomerRecord record;
  request.SetOrder(customer_id, -1, 2);

  timer.Start();
  record = stub.ReadRecord(request);
  timer.EndAndMerge();

  std::stringstream ss;
  if (!record.IsValid()) {
    ss << "customer " << customer_id << " not exsit" << std::endl;
  } else {
    ss << "customer " << customer_id << " last order "
              << record.GetLastOrder() << std::endl;
  }
  std::cout << ss.str();
}

void ClientThreadClass::ScanRecords() {
  for (int i = 0; i < num_orders; i++) {
    CustomerRequest request;
    CustomerRecord record;
    request.SetOrder(i, -1, 2);
    timer.Start();
    record = stub.ReadRecord(request);
    timer.EndAndMerge();
    if (record.IsValid()) {
      std::cout << record.GetCustomerId() << "\t" << record.GetLastOrder()
                << std::endl;
    }
  }
}