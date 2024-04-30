#include "Order.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using std::string;
using std::vector;
using namespace std;

Order::Order(int id, int customerId, int distance) : id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING), collectorId(-1), driverId(-1)
{
}

int Order::getId() const
{

    return id;
}
int Order::getDistance() const
{
    return distance;
}
int Order::getCustomerId() const
{
    return customerId;
}

void Order::setStatus(OrderStatus status)
{
    this->status = status;
}

void Order::setCollectorId(int collectorId)
{
    this->collectorId = collectorId;
}

void Order::setDriverId(int driverId)
{
    this->driverId = driverId;
}

int Order::getCollectorId() const
{
    return collectorId;
}

int Order::getDriverId() const
{
    return driverId;
}

OrderStatus Order::getStatus() const
{
    return status;
}

const string Order::toString() const
{
    return std::to_string(id);
}

string Order::getStringOrderStatus()
{
    string str = "";
    if (this->getStatus() == OrderStatus::COLLECTING)
    {
        str = "COLLECTING";
    }
    else if (this->getStatus() == OrderStatus::PENDING)
    {
        str = "PENDING";
    }
    else if (this->getStatus() == OrderStatus::DELIVERING)
    {
        str = "DELIVERING";
    }
    else if (this->getStatus() == OrderStatus::COMPLETED)
    {
        str = "COMPLETED";

        
    } // additional function
    return str;
}
    Order *Order::clone() const
    {
        return new Order(*this);
    } // Return a copy of the Order
