#ifndef __CLIENT_STUB_H__
#define __CLIENT_STUB_H__

#include <string>

#include "ClientSocket.h"
#include "Messages.h"

class ClientStub {
private:
    ClientSocket socket;
public:
    ClientStub();

    int Init(std::string ip, int port);
 
    LaptopInfo Order(CustomerRequest order); // take a customer request and return laptop information.
    CustomerRecord ReadRecord(CustomerRequest order); // take a customer request and return a customer record.
    LogResponse BackupRecord(LogRequest log);
    void Identify(int role);
};


#endif // end of #ifndef __CLIENT_STUB_H__