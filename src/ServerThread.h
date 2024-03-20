#ifndef __SERVERTHREAD_H__
#define __SERVERTHREAD_H__

#include <condition_variable>
#include <future> // std::promise
#include <mutex>
#include <queue>
#include <thread>

#include "Messages.h"
#include "ServerSocket.h"

struct ExpertRequest {
    LaptopInfo laptop;
    std::promise <LaptopInfo> prom; // To make sure that the pair of regular engineer and the expert engineer who work on the same laptop can exchange the wake up handles and ids, you can design the request to include rich information in addition to the laptop information.
};

class LaptopFactory {
private:
    std::queue <std::unique_ptr<ExpertRequest>> erq;
    std::mutex erq_lock;
    std::condition_variable erq_cv;

    LaptopInfo CreateRegularLaptop(LaptopOrder order, int engineer_id);

    LaptopInfo CreateCustomLaptop(LaptopOrder order, int engineer_id);

public:
    void EngineerThread(std::unique_ptr <ServerSocket> socket, int id);

    void ExpertThread(int id);
};

#endif // end of #ifndef __SERVERTHREAD_H__

