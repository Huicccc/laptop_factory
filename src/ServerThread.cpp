#include <iostream>
#include <memory> // For std::unique_ptr

#include "ServerThread.h"
#include "ServerStub.h"

// Create a regular laptop based on the order and engineer's ID
LaptopInfo LaptopFactory::CreateRegularLaptop(LaptopOrder order, int engineer_id) {
    LaptopInfo laptop; // Initialize an empty laptop info structure
    laptop.CopyOrder(order); // Copy the order details into the laptop info
    laptop.SetEngineerId(engineer_id); // Set the ID of the engineer handling the order
    laptop.SetExpertId(-1); // Indicate that no expert was involved (-1)
    return laptop; // Return the completed laptop info
}

// Create a custom laptop, involving asynchronous processing by an expert
LaptopInfo LaptopFactory::CreateCustomLaptop(LaptopOrder order, int engineer_id) {
    LaptopInfo laptop;
    laptop.CopyOrder(order);
    laptop.SetEngineerId(engineer_id); // Similar to CreateRegularLaptop, but with extra steps for custom processing

    std::promise <LaptopInfo> prom; // Promise to hold the result of the expert's work
    std::future <LaptopInfo> fut = prom.get_future(); // Future to asynchronously receive the result

    // Create a request for an expert's review
    std::unique_ptr <ExpertRequest> req = std::unique_ptr<ExpertRequest>(new ExpertRequest);
    req->laptop = laptop; // Attach the current laptop info
    req->prom = std::move(prom); // Move the promise into the request

    // Lock the queue, push the request, notify one expert thread, and unlock
    erq_lock.lock();
    erq.push(std::move(req));
    erq_cv.notify_one();
    erq_lock.unlock();

    laptop = fut.get(); // Wait for the expert to process the request and get the result
    return laptop; // Return the updated laptop info
}

// Thread function for engineers processing laptop orders
void LaptopFactory::EngineerThread(std::unique_ptr <ServerSocket> socket, int id) {
    int engineer_id = id;
    ServerStub stub;
    stub.Init(std::move(socket)); // Initialize the server stub with the socket

    while (true) {
        LaptopOrder order = stub.ReceiveOrder(); // Receive an order
        if (!order.IsValid()) { // Break the loop if the order is invalid
            break;
        }

        int laptop_type = order.GetLaptopType(); // Get the type of laptop ordered
        LaptopInfo laptop; // Placeholder for the laptop info to be sent
        // Process the order based on its type
        switch (laptop_type) {
            case 0: // Regular laptop
                laptop = CreateRegularLaptop(order, engineer_id);
                break;
            case 1: // Custom laptop
                laptop = CreateCustomLaptop(order, engineer_id);
                break;
            default:
                std::cout << "Undefined laptop type: " << laptop_type << std::endl;
        }

        stub.SendLaptop(laptop); // Send the processed laptop info back
    }
}

// Thread function for experts to process custom laptop requests
void LaptopFactory::ExpertThread(int id) {
    std::unique_lock <std::mutex> ul(erq_lock, std::defer_lock); // Prepare a unique lock but do not lock yet
    while (true) {
        ul.lock(); // Lock the queue

        // Wait until there's at least one request in the queue
        if (erq.empty()) {
            erq_cv.wait(ul, [this] { return !erq.empty(); });
        }

        // Move the first request out of the queue and pop it
        auto req = std::move(erq.front());
        erq.pop();

        ul.unlock(); // Unlock the queue

        // Simulate processing time
        std::this_thread::sleep_for(std::chrono::microseconds(100));

        req->laptop.SetExpertId(id); // Set the expert ID on the laptop
        req->prom.set_value(req->laptop); // Fulfill the promise with the processed laptop
    }
}