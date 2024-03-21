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
 
    /*
        2.5 New stubs and communications
        Similar to assignment #1, you should implement necessary stubs that hide the communication details for all network operations. 
        For example, stubs should be created for the send and receive messages in 4.(a) and 4.(c) of Section 2.2 and also for the PFA/customer identification message in Section 2.3.3.    
    */
    LaptopInfo Order(CustomerRequest order); // take a customer request and return laptop information.
    CustomerRecord ReadRecord(CustomerRequest order); // take a customer request and return a customer record.
    LogResponse BackupRecord(LogRequest log);
    void Identify(int role);
};


#endif // end of #ifndef __CLIENT_STUB_H__