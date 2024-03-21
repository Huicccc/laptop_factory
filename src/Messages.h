#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <string>

// class LaptopOrder {
class CustomerRequest {
private:
    int customer_id; // For laptop order: customer id who placed order.
                    // For read request: customer id to read from map.
    int order_number; // # of orders issued by this customer so far
                      // Record-read request does not count as an order.
                      // Set to -1 for read requests.
    int request_type; // Either 1 - regular laptop order request
                      // 2 - customer record read request

public:
    CustomerRequest();

    void operator=(const CustomerRequest &order) {
        customer_id = order.customer_id;
        order_number = order.order_number;
        // laptop_type = order.laptop_type;
        request_type = order.request_type;
    }

    void SetOrder(int cid, int order_num, int type);

    int GetCustomerId();

    int GetOrderNumber();

    int GetLaptopType();

    int Size();

    void Marshal(char *buffer);

    void Unmarshal(char *buffer);

    bool IsValid();

    void Print();
};

class LaptopInfo {
private:
    int customer_id; // copied from the order
    int order_number; // copied from the order for request_type 1
                      // copied from the record map for request_type 2
    // int laptop_type; // copied from the order
    int request_type; // copied from the order
    int engineer_id; // id of the engineer who created the laptop
    // int expert_id; // id of the expert who added a custom module
    //                // -1 indicates that there is no custom module
    int admin_id; // id of the admin who updated the record map
                  // -1 indicates that the request was a read

public:
    LaptopInfo();

    void operator=(const LaptopInfo &info) {
        customer_id = info.customer_id;
        order_number = info.order_number;
        // laptop_type = info.laptop_type;
        request_type = info.request_type;
        engineer_id = info.engineer_id;
        // expert_id = info.expert_id;
        admin_id = info.admin_id;
    }

    // void SetInfo(int cid, int order_num, int type, int engid, int expid);
    void SetInfo(int cid, int order_num, int type, int engid, int admid);
    
    // void CopyOrder(LaptopOrder order);
    void CopyOrder(CustomerRequest order);

    void SetEngineerId(int id);

    // void SetExpertId(int id);
    void SetAdminId(int id);

    int GetCustomerId();

    int GetOrderNumber();

    // int GetLaptopType();
    int GetRequestType();

    int GetEngineerId();

    // int GetExpertId();
    int GetAdminId();

    int Size();

    void Marshal(char *buffer);

    void Unmarshal(char *buffer);

    bool IsValid();

    void Print();
};

class CustomerRecord {
private:
  int customer_id; // copied from the read request
                  // -1 if customer_id is not found in the map
  int last_order; // copied from the map
                  // -1 if customer_id is not found in the map      

public:
  CustomerRecord();
  void operator=(const CustomerRecord &record) {
    customer_id = record.customer_id;
    last_order = record.last_order;
  }

  void SetCustomerRecord(int cid, int order_number);
  int GetCustomerId();
  int GetLastOrder();

  int Size();

  void Marshal(char *buffer);
  void Unmarshal(char *buffer);

  bool IsValid();

  void Print();
};

/*
MapOp is a general structure that defines the operation that can be applied to the map and parameters for the operation. You will only use opcode 1 which updates the map. 
For example, if opcode is 1, arg1 is 100, and arg2 is 20, then this means that find the customer record for customer number 100 in the map and update the value to 20 
(if the customer record does not exist, create a record for the customer number 100 and assign the value 20).
*/
class MapOp {
private:
  int opcode; // operation code: 1 - update value
  int arg1; // customer_id to apply the operation
  int arg2; // parameter for the operation

public:
  MapOp();
  void operator=(const MapOp &op) {
    opcode = op.opcode;
    arg1 = op.arg1;
    arg2 = op.arg2;
  }

  void SetMapOp(int op, int a1, int a2);
  void CopyMapOp(MapOp op);
  void SetOpCode(int op) { opcode = op; }
  void SetArg1(int a1) { arg1 = a1; }
  void SetArg2(int a2) { arg2 = a2; }

  int GetOpCode() { return opcode; }
  int GetArg1() { return arg1; }
  int GetArg2() { return arg2; }

  int Size();

  void Marshal(char *buffer);
  void Unmarshal(char *buffer);

  bool IsValid() { return (opcode != -1); }

  void Print();
};

class LogRequest {
private:
  int factory_id;
  int committed_index;
  int last_index;
  MapOp op;

public:
  LogRequest();
  void operator=(const LogRequest &request) {
    factory_id = request.factory_id;
    committed_index = request.committed_index;
    last_index = request.last_index;
    op = request.op;
  }
  void CopyRequest(LogRequest request);
  void SetFactoryId(int id) { factory_id = id; }
  void SetCommittedIndex(int index) { committed_index = index; }
  void SetLastIndex(int index) { last_index = index; }
  void SetMapOp(MapOp op) { this->op = op; }

  int GetFactoryId() { return factory_id; }
  int GetCommittedIndex() { return committed_index; }
  int GetLastIndex() { return last_index; }
  MapOp GetMapOp() { return op; }

  int Size();

  void Marshal(char *buffer);
  void Unmarshal(char *buffer);

  bool IsValid() { return (factory_id != -1); }

  void Print();
};

class LogResponse {
private:
  int factory_id;
public:
  LogResponse();
  void operator=(const LogResponse &response) {
    factory_id = response.factory_id;
  }
  void SetFactoryId(int id) { factory_id = id; }
  int GetFactoryId() { return factory_id; }

  int Size() { return sizeof(factory_id); }

  void Marshal(char *buffer);
  void Unmarshal(char *buffer);

  bool IsValid() { return (factory_id != -1); }

  void Print();
};

#endif // #ifndef __MESSAGES_H__