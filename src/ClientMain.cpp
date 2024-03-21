#include <array>
#include <iostream>
#include <iomanip>
#include <thread>
#include <vector>
#include <memory>

#include "ClientSocket.h"
#include "ClientThread.h"
#include "ClientTimer.h"

int main(int argc, char *argv[]) {
    std::string ip; // IP address of the server
    int port; // Port number of the server
    int num_customers; // Number of customer threads to create
    int num_orders; // Number of orders each customer will place
    // int laptop_type; // Type of laptop to order (0 for regular, 1 for custom)
	int request_type;
    ClientTimer timer; // Timer for measuring overall operation time

    // Vectors to hold client thread objects and the threads themselves
    std::vector<std::shared_ptr<ClientThreadClass>> client_vector;
    std::vector<std::thread> thread_vector;

    // Check if the correct number of arguments are passed
    if (argc < 6) {
        std::cout << "not enough arguments" << std::endl;
        // std::cout << argv[0] << "[ip] [port #] [# customers] [# orders] [laptop type 0 or 1]" << std::endl;
		std::cout << argv[0] << "[ip] [port #] [# customers] ";
    	std::cout << "[# orders] [request type 1, 2 or 3]" << std::endl;
        return 0; // Exit if not enough arguments
    }

    /*
        You may want to make sure that the server program correctly updates the customer record. To check this, you should add a request type 3 workflow to the client program. Request type 3 only exists as a command line argument for the client program. 
        It is a custom command that lets a customer thread to scan through all customer records and print them on the screen. Request type 3 workflow does not require server program modifications at all, because it only affects the client program behavior.
        When request type 3 is given as the command line argument for the client program, each customer thread issues record read requests as many as # orders to scan all customer records and prints the returned customer record from the server on the screen. 
        We will assume that customer ids are linearly assigned (e.g., if you run 100 customers, ids can start from say 11 and end at 110), and the # orders argument will set the maximum customer id to scan for.
        For example, if the client program is executed with the command below,
        ./client 123.456.789.123 12345 1 128 3
        the client program will spawn one customer thread, and the customer thread will send 128 read request to the server where the customer id varies from 0 to 128, order number is fixed to -1, and request type is fixed to 2. 
        If the server returns customer id = -1, it means the record does not exist, so you can skip printing the record. If a valid record is returned you should print them in the following format:
        // customer_id [tab "\t"] last_order
        11 99
        12 99
        13 99
        Using request type 3, you can read what is stored in your server. For example, you can run the client program with request type 1, 
        and then run the program again with request type 3 to check whether the server is holding the desired customer record.
    */
    ip = argv[1];
    port = atoi(argv[2]);
    num_customers = atoi(argv[3]);
    num_orders = atoi(argv[4]);
    // laptop_type = atoi(argv[5]);
	request_type = atoi(argv[5]);
	if (request_type == 3) {
		num_customers = 1; // Since all customer threads will be issuing the same request redundantly, it is recommended that you use only 1 customer thread when using the request type 3.
	}

    timer.Start(); // Start the timer for the whole operation

    // 1. The program should create the customer threads as many as the specified customer number.
    for (int i = 0; i < num_customers; i++) {
        auto client_cls = std::shared_ptr<ClientThreadClass>(new ClientThreadClass());
        // std::thread client_thread(&ClientThreadClass::ThreadBody, client_cls, ip, port, i, num_orders, laptop_type);
		std::thread client_thread(&ClientThreadClass::ThreadBody, client_cls, ip, port, i, num_orders, request_type);

        client_vector.push_back(std::move(client_cls)); // Store client class instance
        thread_vector.push_back(std::move(client_thread)); // Store thread
    }

    // Join all threads to ensure they complete before proceeding
    for (auto &th : thread_vector) {
        th.join();
    }
    timer.End(); // End the timer after all threads have completed

    // Merge the timers from all client threads to calculate overall stats
    for (auto &cls : client_vector) {
        timer.Merge(cls->GetTimer());
    }

    timer.PrintStats(); // Print the collected timing statistics

    return 1; // Return success
}