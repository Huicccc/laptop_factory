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
    std::map<int, int> customer_record;
    std::map<int, ServerAddress> admin_map;
    bool admin_stub_init;
    std::map<int, ClientStub> admin_stub;
    std::vector<MapOp> smr_log;

    int last_index;
    int committed_index;
    int primary_id;
    int factory_id;

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

