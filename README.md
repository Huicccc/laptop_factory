### Laptop Ordering System.
The client can place orders for laptops, specifying whether they want a regular laptop or a custom laptop. The server, representing a laptop factory, processes these orders. Custom laptops require attention from expert engineers, a resource modeled to be scarce within the system. This system simulates a complex client-server interaction with an emphasis on concurrency and synchronization. It demonstrates handling of real-world challenges such as resource scarcity (expert engineers) and the need for efficient inter-thread communication and synchronization in a multi-threaded environment.

### Client-Server Architecture Overview
- **Client Side**: Multiple customer threads are created to simulate different customers placing orders. Each customer can place several orders, and each order specifies the type of laptop desired (regular or custom).
- **Server Side**: The server accepts connections from clients and processes orders. It has a pool of regular engineers to assemble regular laptops and a limited number of expert engineers for customizing laptops as per orders.

### Key Components and Workflow

#### Client Workflow:
1. **Initialization**: The client program takes command line arguments specifying the server's IP address, port number, number of customers, number of orders per customer, and the type of laptop.
2. **Order Placement**: Each customer thread places orders for laptops. The number of orders and the type of laptop (regular or custom) are determined by the input arguments.
3. **Timing Statistics**: The client measures the latency for each order using `ClientTimer`. This includes average latency, minimum latency, maximum latency, and throughput, which are printed at the end of execution.

#### Server Workflow:
1. **Expert Engineer Thread Pool**: A pool of expert engineers is created based on the input argument specifying the number of expert engineers. These engineers are responsible for customizing laptops.
2. **Order Processing**:
   - Regular laptops are processed by regular engineers without needing expert intervention.
   - Custom laptops require the regular engineer to place a request in a shared FIFO queue for expert engineers. An expert engineer then picks up the request, customizes the laptop, and notifies the regular engineer upon completion.
3. **Synchronization**: The implementation includes mechanisms for thread-safe interaction between regular and expert engineers using mutexes, condition variables, and futures/promises for asynchronous result handling.

#### Communication:
- The client and server communicate using TCP sockets. The client stub and server stub abstract the socket communication, handling the marshalling and unmarshalling of order and laptop information.

#### Customization Workflow:
- When a custom laptop order is received, the regular engineer enqueues a request for an expert engineer and waits. An expert engineer customizes the laptop, sets their ID in the laptop information, and signals completion. The regular engineer then sends the finalized laptop information back to the client.

### Concurrency and Synchronization:
- **Concurrency**: Managed through the use of multiple threads for customer simulation, regular engineers, and expert engineers.
- **Synchronization**: Achieved using mutexes and condition variables to safely access shared resources (like the expert request queue) and futures/promises to synchronize the custom laptop customization process.

### Performance Metrics:
- The code includes a `ClientTimer` class to measure and report the timing statistics for order processing, such as average latency, minimum latency, maximum latency, and throughput.
