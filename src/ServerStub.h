#ifndef __SERVER_STUB_H__
#define __SERVER_STUB_H__

#include <memory>

#include "ServerSocket.h"
#include "Messages.h"

class ServerStub {
private:
    std::unique_ptr <ServerSocket> socket;
public:
    ServerStub();

    void Init(std::unique_ptr <ServerSocket> socket);

    // LaptopOrder ReceiveOrder();
    CustomerRequest ReceiveRequest();

    int SendLaptop(LaptopInfo info); 

    int ReturnRecord(CustomerRecord record); // take a customer record and send the customer record

    LogRequest ReceiveLogRequest();

    int ReturnLogResponse(LogResponse response);
    
    int ReceiveIndentity();
};

#endif // end of #ifndef __SERVER_STUB_H__