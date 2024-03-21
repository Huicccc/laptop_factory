#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <string>

// class LaptopOrder {
class CustomerRequest {
private:
    int customer_id; // customer id
    int order_number; // # of orders issued by this customer so far
    // int laptop_type; // either 0 - regular or 1 - custom
    int request_type;

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
    int order_number; // copied from the order
    // int laptop_type; // copied from the order
    int request_type;
    int engineer_id; // id of the engineer who created the laptop
    // int expert_id; // id of the expert who added a custom module
    //                // -1 indicates that there is no custom module
    int admin_id;

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
  int customer_id;
  int last_order;

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

class MapOp {
private:
  int opcode;
  int arg1;
  int arg2;

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