// #include <chrono>
#include <iostream>
// #include <mutex>
#include <thread>
#include <vector>

#include "ServerSocket.h" 
#include "ServerThread.h"

int main(int argc, char *argv[]) {
    int port;
    int engineer_cnt = 0;
    int admin_id = -1;
    int num_peers = 0;
    ServerSocket socket;
    LaptopFactory factory;
    std::unique_ptr<ServerSocket> new_socket;
    std::vector<std::thread> thread_vector;

    /*
        You were only selling regular laptops with no customization and you want to add customized laptops to your production line. 
        Custom laptops require an expert engineer who can add and test customized module to the regular laptop. 
        However, expert engineers are difficult to hire so you cannot assign them per customer. 
        Therefore, to create a custom laptop, engineers should create the regular laptop and request the expert engineer to attach the custom module. 
        Because there are only a few expert engineers, requests should be queued and processed as the expert engineer becomes available.

        Adding expert engineer workflow to laptop factory
        To add expert engineer workflow, the server program should take an additional command line argument, which is the number of expert engineers.
        ./server [port #] [# experts]
        For example,
        ./ server 12345 2
        indicates that the server uses port number 12345 and has 2 expert engineers.
    */
    if (argc < 4) {
        usage(argv);
        return 0;
    }
    admin_id = atoi(argv[2]);
    num_peers = atoi(argv[3]);

    if (argc < 4 + num_peers * 3) {
        usage(argv);
        return 0;
    }

    /*
        Expert engineers should be added to the server program as a thread pool. Recall that threads in a thread pool wait for tasks to arrive using guards (condition variables). 
        You should use a single FIFO queue that is shared among all expert engineer threads and regular engineer threads. 
        The regular engineers will enqueue requests to the queue and the expert engineers will dequeue the request, process it, and notify completion to the engineer who requested the task. 
        The request sent to the expert engineer should include the laptop information that the engineer was working on, and the response by the expert engineer should send back the laptop information with the expert engineer’s id filled in.
        
        Once the regular engineer issued a request to expert engineers, it should wait until an expert engineer notifies completion of the request, and then send back the laptop information back to the customer.
        Each regular engineer thread should look at the customer order and send expert engineer re- quests only if the laptop type is custom. For regular laptop types, the engineer should solely create the laptop without expert engineer’s involvement as described in Section 2.
        
        Now, the main thread of the server program should be modified. The following indicates additional functions that you need to implement.
        1. The server program creates the expert engineer request queue and the expert engineer thread pool. Expert engineer threads should be given unique ids that do not overlap with regular engineer ids.
        2. The regular engineer threads should be given ways to access the expert request queue so that requests can be sent to the expert engineer threads.
    */
    // for (int i = 0; i < num_experts; i++) {
    //     std::thread expert_thread(&LaptopFactory::ExpertThread,
    //                               &factory, engineer_cnt++);
    //     thread_vector.push_back(std::move(expert_thread)); // expert engineer workflow 2. An expert engineer thread pool.
    // }


    /*
        Based on years of order history, you know that every new customer will place many orders. 
        So you decided to hire and assign an engineer who produces laptops for each customer. 
        Currently, your laptop factory only produces regular type laptops.
        The main thread of the server program should work as the following.
        1. The server program creates sockets to accept new connections and waits for new connections from client programs in a loop.
        2. Once a new connection is made, it creates a new engineer thread for the connection. 
        The engineer thread should be given access to a server stub that uses the new connection to directly communicate with the customer thread.
    */    
    if (!socket.Init(port)) {
        std::cout << "Socket initialization failed" << std::endl;
        return 0;
    }

    for (int i = 0; i < num_peers; i++) {
        int id = atoi(argv[4 + i * 3]);
        std::string ip = argv[5 + i * 3];
        int port = atoi(argv[6 + i * 3]);
        factory.AddAdmin(id, ip, port);
    }

    thread_vector.push_back(std::thread(&LaptopFactory::AdminThread, &factory, admin_id));    

    while ((new_socket = socket.Accept())) {
        std::thread engineer_thread(&LaptopFactory::EngineerThread,
                                    &factory, std::move(new_socket),
                                    engineer_cnt++);
        thread_vector.push_back(std::move(engineer_thread)); // expert engineer workflow 1. A shared request queue.
    }

    return 0;
}

/*
    The number of admins per factory is fixed to 1 so the server program does not need to take the relevant argument. 
    However, there are many additional command line arguments that the server program should take. The following summarizes the set of arguments that are necessary:
    1. Port number,
    2. Unique ID (≥ 0) of the factory,
    3. Number of peer factory servers, and
    4. Unique ID, IP, and port of all peer factory servers.
    That is, the execution of the server program should follow the format,
    ./server [port #] [unique ID] [# peers] (repeat [ID] [IP] [port #])
    For example, let’s assume you are running three servers with configurations: 
    • Server ID: 0, IP: 11.11.11.11, port: 12345
    • Server ID: 1, IP: 22.22.22.22, port: 12345
    • Server ID: 2, IP: 33.33.33.33, port: 12345
    Then the three servers should run with the commands: 5
    ./server 12345 0 2 1 22.22.22.22 12345 2 33.33.33.33 12345 
    ./server 12345 1 2 0 11.11.11.11 12345 2 33.33.33.33 12345 
    ./server 12345 2 2 0 11.11.11.11 12345 1 22.22.22.22 12345
    This allows servers to find each other, and send and receive data replication requests. Through the command line argument, you should be able to configure your servers to work with arbitrary number of peers. 
    Note that the server ID, IP, and port mapping given to each server as arguments should be consistent across all servers.
*/
void usage(char *argv[]) {
  std::cout << "Usage: " << argv[0]
            << " [port #] [unique ID] [# peers] (repeat [ID] [IP] [port #])"
            << std::endl;
}