#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <string>

class LaptopOrder {
private:
    int customer_id; // customer id
    int order_number; // # of orders issued by this customer so far
    int laptop_type; // either 0 - regular or 1 - custom

public:
    LaptopOrder();

    void operator=(const LaptopOrder &order) {
        customer_id = order.customer_id;
        order_number = order.order_number;
        laptop_type = order.laptop_type;
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
    int laptop_type; // copied from the order
    int engineer_id; // id of the engineer who created the laptop
    int expert_id; // id of the expert who added a custom module
                   // -1 indicates that there is no custom module

public:
    LaptopInfo();

    void operator=(const LaptopInfo &info) {
        customer_id = info.customer_id;
        order_number = info.order_number;
        laptop_type = info.laptop_type;
        engineer_id = info.engineer_id;
        expert_id = info.expert_id;
    }

    void SetInfo(int cid, int order_num, int type, int engid, int expid);

    void CopyOrder(LaptopOrder order);

    void SetEngineerId(int id);

    void SetExpertId(int id);

    int GetCustomerId();

    int GetOrderNumber();

    int GetLaptopType();

    int GetEngineerId();

    int GetExpertId();

    int Size();

    void Marshal(char *buffer);

    void Unmarshal(char *buffer);

    bool IsValid();

    void Print();
};

#endif // #ifndef __MESSAGES_H__