#include "Customer.h"
#include "Order.h"
#include <iostream>
class wareHouse;

using namespace std;
Customer::Customer(int id, const string &name, int locationDistance, int maxOrders) : id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders), numOfOrdersLeft(maxOrders), ordersId()
{
}

const string &Customer::getName() const
{
    return name;
}

int Customer::getId() const
{
    return id;
}

int Customer::getCustomerDistance() const
{
    return locationDistance;
}

int Customer::getMaxOrders() const
{
    return maxOrders;
}
// Returns maxOrders
int Customer::getNumOrders() const
{
    return ordersId.size();
}
// Returns num of orders the customer has made so far
bool Customer::canMakeOrder() const
{
    return !(maxOrders == getNumOrders());
}
// Returns true if the customer didn't reach max orders
const vector<int> &Customer::getOrdersids() const
{
    return ordersId;
}

int Customer::addOrder(int orderId)
{
    if (canMakeOrder())
    {
        Order newOrder = Order(orderId, id, locationDistance);
        ordersId.push_back(orderId);
        numOfOrdersLeft--;
        return orderId;
    }
    return -1;
} // return OrderId if order was added successfully, -1 otherwise
int Customer::getNumOfOrdersLeft(){
    return numOfOrdersLeft;
}

SoldierCustomer::SoldierCustomer(int id, string name, int locationDistance, int maxOrders) : Customer(id, name, locationDistance, maxOrders)
{
}
SoldierCustomer *SoldierCustomer::clone() const
{
    return new SoldierCustomer(*this);
}
CivilianCustomer::CivilianCustomer(int id, string name, int locationDistance, int maxOrders) : Customer(id, name, locationDistance, maxOrders)
{
}

CivilianCustomer *CivilianCustomer::clone() const
{
    return new CivilianCustomer(*this);
}

// Return a copy of the customer
