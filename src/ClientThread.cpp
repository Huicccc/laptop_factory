#include "ClientThread.h"
#include "Messages.h"

#include <iostream>

// Default constructor implementation
ClientThreadClass::ClientThreadClass() {}

// Implementation of ThreadBody, which executes the main functionality of the client thread
void ClientThreadClass::
ThreadBody(std::string ip, int port, int id, int orders, int type) {
    customer_id = id; // Set the customer ID
    num_orders = orders; // Set the number of orders to process
    laptop_type = type; // Set the laptop type to order

    // Initialize the network stub for communication with the server
    if (!stub.Init(ip, port)) {
        std::cout << "Thread " << customer_id << " failed to connect" << std::endl;
        return; // Exit if connection to the server fails
    }

    // Loop over each order
    for (int i = 0; i < num_orders; i++) {
        LaptopOrder order; // Create a new order
        LaptopInfo laptop; // Create a variable to hold the received laptop info
        order.SetOrder(customer_id, i, laptop_type); // Set up the order with details

        timer.Start(); // Start timing the order process
        laptop = stub.OrderLaptop(order); // Send the order and receive the laptop info
        timer.EndAndMerge(); // Stop timing and merge the result into the timer

        // Check if the received laptop info is valid
        if (!laptop.IsValid()) {
            std::cout << "Invalid laptop " << customer_id << std::endl;
            break; // Break out of the loop if an invalid laptop is received
        }
    }
}

// Returns a copy of the timer object, allowing external access to timing data
ClientTimer ClientThreadClass::GetTimer() {
    return timer;
}