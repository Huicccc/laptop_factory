#include <array>
#include <iostream>
#include <iomanip>
#include <thread>
#include <vector>

#include "ClientSocket.h"
#include "ClientThread.h"
#include "ClientTimer.h"

int main(int argc, char *argv[]) {
    std::string ip; // IP address of the server
    int port; // Port number of the server
    int num_customers; // Number of customer threads to create
    int num_orders; // Number of orders each customer will place
    int laptop_type; // Type of laptop to order (0 for regular, 1 for custom)
    ClientTimer timer; // Timer for measuring overall operation time

    // Vectors to hold client thread objects and the threads themselves
    std::vector<std::shared_ptr<ClientThreadClass>> client_vector;
    std::vector<std::thread> thread_vector;

    // Check if the correct number of arguments are passed
    if (argc < 6) {
        std::cout << "not enough arguments" << std::endl;
        std::cout << argv[0] << "[ip] [port #] [# customers] [# orders] [laptop type 0 or 1]" << std::endl;
        return 0; // Exit if not enough arguments
    }

    // Parse command line arguments
    ip = argv[1];
    port = atoi(argv[2]);
    num_customers = atoi(argv[3]);
    num_orders = atoi(argv[4]);
    laptop_type = atoi(argv[5]);

    timer.Start(); // Start the timer for the whole operation

    // 1. The program should create the customer threads as many as the specified customer number.
    for (int i = 0; i < num_customers; i++) {
        auto client_cls = std::shared_ptr<ClientThreadClass>(new ClientThreadClass());
        std::thread client_thread(&ClientThreadClass::ThreadBody, client_cls, ip, port, i, num_orders, laptop_type);

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