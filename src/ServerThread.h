#ifndef __SERVERTHREAD_H__
#define __SERVERTHREAD_H__

#include <condition_variable>
#include <future> // std::promise
#include <mutex>
#include <queue>
// #include <thread>
#include <map>

#include "Messages.h"
#include "ServerSocket.h"
#include "ClientStub.h"

// struct ExpertRequest {
struct AdminRequest {
    LaptopInfo laptop;
    std::promise <LaptopInfo> prom; // To make sure that the pair of regular engineer and the expert engineer who work on the same laptop can exchange the wake up handles and ids, you can design the request to include rich information in addition to the laptop information.
};

struct ServerAddress {
  std::string ip;
  int port;
};


class LaptopFactory {
private:
    // std::queue <std::unique_ptr<ExpertRequest>> erq;
    std::queue<std::unique_ptr<AdminRequest>> erq;
    std::mutex erq_lock;
    std::condition_variable erq_cv;

    std::mutex cr_lock;
    std::map<int, int> customer_record; // The key of the map is the customer id, and the value is the last order number of the customer.
    std::map<int, ServerAddress> admin_map;
    bool admin_stub_init;
    std::map<int, ClientStub> admin_stub;
    std::vector<MapOp> smr_log; // state machine replication log

    /*
    MapOps in the smr_log are not applied to the customer record immediately, 
    but are applied only after the MapOp has been replicated to all nodes; 
    we regard such fully replicated MapOp as “committed.” 

    In addition to the smr_log and the customer_record, all nodes keep track of the last written index of the smr_log, 
    up to which index the MapOps in the smr_log has been committed, and which server is the production factory:

    Because of communication delays, the information in each node can be slightly different. You may add additional variables and data structure as needed.
    */
    int last_index; // the last index of the smr_log that has data
    int committed_index; // the last index of the smr_log where the MapOp of the log entry is committed and applied to the customer_record
    int primary_id; // the production factory id (server id) initially set to -1.
    int factory_id; // the id of the factory. This is assigned via the command line arguments.

    // LaptopInfo CreateRegularLaptop(LaptopOrder order, int engineer_id);
    LaptopInfo CreateRegularLaptop(CustomerRequest order, int engineer_id);
    // LaptopInfo CreateCustomLaptop(LaptopOrder order, int engineer_id);
    CustomerRecord CreateCustomerRecord(CustomerRequest request);
    LogRequest CreateLogRequest(MapOp op);
    void PFA(LaptopInfo& laptop);

public:
    void EngineerThread(std::unique_ptr <ServerSocket> socket, int id);

    // void ExpertThread(int id);
    void AddAdmin(int id, std::string ip, int port);
    
    void AdminThread(int id);
};



#endif // end of #ifndef __SERVERTHREAD_H__

