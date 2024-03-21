#include <iostream>
#include <memory> // For std::unique_ptr

#include "ServerThread.h"
#include "ServerStub.h"

/*
  1. The laptop manufacturing workflow should now be:
  (a) A customer sends a laptop order to the (regular) engineer. 
  (b) The engineer creates a laptop (fills in laptop info).
  (c) The engineer sends request to the admin to update the customer record and waits. 
  (d) The admin who was waiting for requests fetches the request.
  (e) The admin appends a necessary operation to update the customer record to the log. 
  (f) The admin then applies the logged operation to the customer record map.
  (g) The admin notifies record update completion with its id to the waiting engineer. 
  (h) The engineer wakes up and ships the laptop to the customer.
  2. The customer record reading workflow should be:
  (a) A customer sends a record read request for its own record (a customer cannot access other customer records by default, but you will break this rule later).
  (b) The engineer reads the corresponding record from the customer record map.
  (c) The engineer returns the record to the customer.
*/

// LaptopInfo LaptopFactory::CreateRegularLaptop(CustomerRequest order, int engineer_id) {
//     LaptopInfo laptop; 
//     laptop.CopyOrder(order);
//     laptop.SetEngineerId(engineer_id);
//     laptop.SetExpertId(-1); // It copies the information in the order to the laptop information and adds its own engineer id and expert id (use value -1 for now).
//     return laptop;
// }
CustomerRecord LaptopFactory::CreateCustomerRecord(CustomerRequest request) {
  CustomerRecord record;
  int customer_id = request.GetCustomerId();
  int last_order = -1;
  {
    std::lock_guard<std::mutex> lock(cr_lock);
    if (customer_record.find(customer_id) != customer_record.end()) {
      last_order = customer_record[customer_id];
    } else {
      last_order = -1;
      customer_id = -1;
    }
  }
  record.SetCustomerRecord(customer_id, last_order);
  return record;
}

/*
    the expert engineer must return the response to the regular engineer who initially sent the request; this ensures that the same regular engineer who received the request from the customer can return the final response back to the client. 
    The key components to support the expert engineer workflow are implementing:
    1. A shared request queue.
    2. An expert engineer thread pool.
    3. Mechanisms for regular engineers to enqueue requests to the request queue safely.
    4. Mechanisms for expert engineers to wake up and process the request only when there are pending requests in the queue.
    5. Mechanisms for regular engineers to wait until any expert engineer processes the request. 
    6. Mechanisms for expert engineers to wake up the specific engineer who sent the request.
    7. Mechanisms for expert engineers to notify their ids to the regular engineer who sent the request.
*/
// LaptopInfo LaptopFactory::CreateCustomLaptop(LaptopOrder order, int engineer_id) { 
LaptopInfo LaptopFactory::CreateRegularLaptop(CustomerRequest order, int engineer_id) {    
    LaptopInfo laptop;
    laptop.CopyOrder(order);
    laptop.SetEngineerId(engineer_id);

    laptop.SetAdminId(-1);

    // 7. Mechanisms for expert engineers to notify their ids to the regular engineer who sent the request. promise - future
    std::unique_ptr <AdminRequest> req = std::unique_ptr<AdminRequest>(new AdminRequest);
    std::promise <LaptopInfo> prom;
    std::future <LaptopInfo> fut = prom.get_future(); // Future to asynchronously receive the result
    req->laptop = laptop; // Attach the current laptop info
    req->prom = std::move(prom); // Move the promise into the request

    // 3. Mechanisms for regular engineers to enqueue requests to the request queue safely.
    erq_lock.lock(); // std::mutex erq_lock
    erq.push(std::move(req)); // std::queue <std::unique_ptr<ExpertRequest>> erq -- A shared request queue 
    erq_cv.notify_one(); // std::condition_variable erq_cv
                    
    erq_lock.unlock();

    laptop = fut.get(); // 5. Mechanisms for regular engineers to wait until any expert engineer processes the request. 
    return laptop; // Return the updated laptop info
}

/*
    Since your server waits for clients in a loop, you can type Ctrl+c in the terminal to terminate your server program.
    Each engineer thread implements a loop that continuously processes orders until the client closes the connection. The thread uses the server stub to communicate with the client.
    1. The thread waits for the client’s order to arrive.
    2. Once the order is received, it starts filling in the laptop information for the order. It copies the information in the order to the laptop information and adds its own engineer id and expert id (use value -1 for now). 
    3. Then the laptop information is sent back to the client.

    If the connection is closed by the client, the thread can escape the loop and terminate. A client closing the connection can be noticed by the return value of socket recv() function. 
    When recv error occurs (including client connection close) recv returns -1. You can figure out more details about the recv errors by inspecting the errno (https://man7.org/linux/man-pages/man2/recv.2.html, https://man7.org/linux/man-pages/man3/errno.3.html).
*/
void LaptopFactory::EngineerThread(std::unique_ptr <ServerSocket> socket, int id) {
    int engineer_id = id;

    int request_type;
    CustomerRequest request;
    CustomerRecord record;
    LogRequest log_req;
    LaptopInfo laptop;

    ServerStub stub; // The thread uses the server stub to communicate with the client.
    stub.Init(std::move(socket)); 
    int identity = stub.ReceiveIndentity();
    // std::cout << "Engineer " << engineer_id << " is " << identity << std::endl;  
    if (identity == 0) {
        while (true) { // Each engineer thread implements a loop that continuously processes orders until the client closes the connection. 
            request = stub.ReceiveRequest(); // 1. The thread waits for the client’s order to arrive.
            if (!request.IsValid()) { // Break the loop if the order is invalid
                break;
            }

            // The regular engineer thread should be modified so that it can communicate with the expert engineers, when needed. Necessary additions include,
            // 1. Checking the laptop type in the order and if it is a regular laptop type, then follow the workflow in the previous section.
            // 2. If the laptop type is custom, then send a request to expert engineer threads by enqueuing the request to the expert engineer request queue. 
            // Next, wait for an expert engineer thread to respond back. Once the response is received, the regular engineer thread sends back the laptop information with added expert id to the customer.
            // int laptop_type = order.GetLaptopType();
            // LaptopInfo laptop;
            request_type = request.GetRequestType();
            switch (request_type) {
                // case 0: // Regular laptop
                //     laptop = CreateRegularLaptop(order, engineer_id); // 2. Once the order is received, it starts filling in the laptop information for the order. 
                //     break;
                // case 1: // Custom laptop
                //     laptop = CreateCustomLaptop(order, engineer_id);
                //     break;
                // default:
                //     std::cout << "Undefined laptop type: " << laptop_type << std::endl;
            case 1:
                laptop = CreateRegularLaptop(request, engineer_id);
                stub.SendLaptop(laptop);
                break;
            case 2:
                record = CreateCustomerRecord(request);
                stub.ReturnRecord(record);
                break;
            default:
                std::cout << "Undefined laptop type: " << request_type << std::endl;                
            }
            // stub.SendLaptop(laptop); // 3. Then the laptop information is sent back to the client.
        }        
    } else if (identity == 1) {  // IFA
        while (true) {
            log_req = stub.ReceiveLogRequest();
            if (!log_req.IsValid()) {
                break;
            }
            if (primary_id != log_req.GetFactoryId()) {
                primary_id = log_req.GetFactoryId();
            }
            MapOp op = log_req.GetMapOp();
            smr_log.emplace_back(op);
            last_index = log_req.GetLastIndex();
            committed_index = log_req.GetCommittedIndex();
            if (committed_index >= 0) {
                std::lock_guard<std::mutex> lock(cr_lock);
                MapOp to_apply = smr_log[committed_index];
                customer_record[to_apply.GetArg1()] = to_apply.GetArg2();
            }

            LogResponse resp;
            resp.SetFactoryId(factory_id);
            stub.ReturnLogResponse(resp);
        }
    } else {
        std::cout << "Undefined identity: " << identity << std::endl;
    }  
}

// Each expert engineer thread in the thread pool should work as follows.
// 1. It waits for requests to arrive in the queue.
// 2. Once a request is detected and successfully received, the expert engineer thread works on the laptop for at least 100 microseconds (implement this using std::this thread::sleep for()) and add expert id into the laptop information. 
// Then it responds back to the regular engineer thread that originally sent the request with the completed laptop information.
void LaptopFactory::AdminThread(int id) {
    last_index = -1;
    committed_index = -1;
    primary_id = -1;
    factory_id = id;
    admin_stub_init = false;    

    std::unique_lock <std::mutex> ul(erq_lock, std::defer_lock);
    while (true) { // 1. It waits for requests to arrive in the queue. 2. Once a request is detected and successfully received
        ul.lock();

        if (erq.empty()) {
            erq_cv.wait(ul, [this] { return !erq.empty(); }); // 4. Mechanisms for expert engineers to wake up and process the request only when there are pending requests in the queue.
        }

        auto req = std::move(erq.front());
        erq.pop();

        ul.unlock();

        std::this_thread::sleep_for(std::chrono::microseconds(100)); // the expert engineer thread works on the laptop for at least 100 microseconds (implement this using std::this thread::sleep for())

        // req->laptop.SetExpertId(id); // 7. Mechanisms for expert engineers to notify their ids to the regular engineer who sent the request.
        // req->prom.set_value(req->laptop); // 6. Mechanisms for expert engineers to wake up the specific engineer who sent the request. fut.get();
        PFA(req->laptop);
        req->laptop.SetAdminId(id);
        req->prom.set_value(req->laptop);
    }
}

void LaptopFactory::AddAdmin(int id, std::string ip, int port) {
  ServerAddress addr;
  addr.ip = ip;
  addr.port = port;
  admin_map[id] = addr;
}

LogRequest LaptopFactory::CreateLogRequest(MapOp op) {
  LogRequest request;
  request.SetFactoryId(factory_id);
  request.SetLastIndex(last_index);
  request.SetCommittedIndex(committed_index);
  request.SetMapOp(op);
  return request;
}

void LaptopFactory::PFA(LaptopInfo &laptop) {
  if (primary_id != factory_id) {
    primary_id = factory_id;
  }

  if (!admin_stub_init) {
    for (auto &admin : admin_map) {
      const ServerAddress &addr = admin.second;
      int ret = admin_stub[admin.first].Init(addr.ip, addr.port);
      if (!ret) {
        // std::cout << "Failed to connect to admin " << admin.first << std::endl;
      } else {
        admin_stub[admin.first].Identify(1);
      }
    }
    admin_stub_init = true;
  }

  MapOp op;
  op.SetMapOp(1, laptop.GetCustomerId(), laptop.GetOrderNumber());
  smr_log.emplace_back(op);
  last_index = smr_log.size() - 1;

  LogRequest request = CreateLogRequest(op);
  for (auto iter = admin_stub.begin(); iter != admin_stub.end();) {
    LogResponse resp = iter->second.BackupRecord(request);
    if (!resp.IsValid()) {
      // std::cout << "Failed to backup record to admin " << iter->first
      //           << std::endl;
      iter = admin_stub.erase(iter);
    } else {
      ++iter;
    }
  }
  {
    std::lock_guard<std::mutex> lock(cr_lock);
    customer_record[laptop.GetCustomerId()] = laptop.GetOrderNumber();
  }
  committed_index = last_index;
}
