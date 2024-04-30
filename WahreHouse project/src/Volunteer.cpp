#include <string>
#include <vector>
#include "Order.h"
#include "Volunteer.h"
using std::string;
using std::vector;

Volunteer::Volunteer(int id, const string &name) : completedOrderId(0), activeOrderId(-1), id(id), name(name) {}

int Volunteer::getId() const
{
    return id;
}
const string &Volunteer::getName() const
{
    return name;
}
int Volunteer::getActiveOrderId() const
{
    return activeOrderId;
}
int Volunteer::getCompletedOrderId() const
{
    return completedOrderId;
}
bool Volunteer::isBusy() const
{
    return !(activeOrderId == -1);
} // Signal whether the volunteer is currently processing an order

//------------------------------------------------------------------------------------------------------

CollectorVolunteer::CollectorVolunteer(int id, string name, int coolDown) : Volunteer(id, name), coolDown(coolDown), timeLeft(0)
{
}

CollectorVolunteer *CollectorVolunteer::clone() const
{
    return new CollectorVolunteer(*this);
}

int CollectorVolunteer::getCoolDown() const
{
    return coolDown;
}
int CollectorVolunteer::getTimeLeft() const
{
    return timeLeft;
}
void CollectorVolunteer::setTimeLeftToCoolDown() {
    timeLeft = coolDown;
}//additional function

bool CollectorVolunteer::decreaseCoolDown()
{
    if (!(timeLeft == 0))
    {
        timeLeft = timeLeft -1;
        if (timeLeft == 0)
        {
            completedOrderId++;
            activeOrderId = -1; // the volunteer dont have an active order right now
            return true;
        }
    }
    return false;

} // Decrease timeLeft by 1,return true if timeLeft=0,false otherwise
bool CollectorVolunteer::hasOrdersLeft() const
{
    return true;
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const
{
    return !isBusy();
}

void CollectorVolunteer::acceptOrder(const Order &order)
{
    if (canTakeOrder(order))
    {
        activeOrderId = order.getId();
        timeLeft = coolDown;
    }
}
string CollectorVolunteer::toString() const
{
    return "Collector";
}

void CollectorVolunteer::step()
{
    if (isBusy())
    {
        timeLeft--;
        if (timeLeft == 0)
        {
            completedOrderId++;
            activeOrderId = -1;
        }
    } // if he is not busy,there is no step to do
} 

//------------------------------------------------------------------------------------------------------

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, string name, int coolDown, int maxOrders): CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders)
{
}

LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const
{
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const
{
    return ordersLeft > 0;
}
bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const
{
    if (!isBusy())
    {
        return ordersLeft > 0;
    }
    return false;
}
void LimitedCollectorVolunteer::acceptOrder(const Order &order)
{
    if (canTakeOrder(order))
    {
        activeOrderId = order.getId();
        ordersLeft--;
        setTimeLeftToCoolDown();
    }
}
int LimitedCollectorVolunteer::getMaxOrders() const
{
    return maxOrders;
}
int LimitedCollectorVolunteer::getNumOrdersLeft() const
{
    return ordersLeft;
}
string LimitedCollectorVolunteer::toString() const
{
    return "LimitedCollector";
}
void LimitedCollectorVolunteer::step() {
    
    if (isBusy())
    {
        decreaseCoolDown();
    } 

} //additional func

//------------------------------------------------------------------------------------------------------

DriverVolunteer::DriverVolunteer(int id, string name, int maxDistance, int distancePerStep) : Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(-1)
{

    // while he doesnt have a package to deliver it on -1.
}
DriverVolunteer *DriverVolunteer::clone() const
{
    return new DriverVolunteer(*this);
}

int DriverVolunteer::getDistanceLeft() const
{
    return distanceLeft;
}
void DriverVolunteer::setDistanceLeft(int distanceLeft) {
    this->distanceLeft = distanceLeft;
}
int DriverVolunteer::getMaxDistance() const
{
    return maxDistance;
}
int DriverVolunteer::getDistancePerStep() const
{
    return distancePerStep;
}
bool DriverVolunteer::decreaseDistanceLeft()
{
    this->distanceLeft = distanceLeft - distancePerStep;
    return distanceLeft <= 0;

} // Decrease distanceLeft by distancePerStep,return true if distanceLeft<=0,false otherwise
bool DriverVolunteer::hasOrdersLeft() const
{
    return true;
}
bool DriverVolunteer::canTakeOrder(const Order &order) const
{
    if (!isBusy())
    {
        return maxDistance >= order.getDistance();
    }
    return false;
} // Signal if the volunteer is not busy and the order is within the maxDistance
void DriverVolunteer::acceptOrder(const Order &order) 
{
    if (canTakeOrder(order))
    {
        distanceLeft = order.getDistance();
        activeOrderId = order.getId();
    }

}
void DriverVolunteer::step()
{
    if (isBusy())
    {
        this->distanceLeft = distanceLeft - distancePerStep;
        if (distanceLeft <= 0)
        {
            completedOrderId++;
            activeOrderId = -1;
            distanceLeft = -1; // he is with no delivery at the momemnt
        }
    }

} // Decrease distanceLeft by distancePerStep
string DriverVolunteer::toString() const
{
    return "Driver";
}



//------------------------------------------------------------------------------------------------------

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep, int maxOrders):DriverVolunteer(id, name, maxDistance, distancePerStep) , maxOrders(maxOrders), ordersLeft(maxOrders)
{
   
   
}


LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const
{
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const
{
    return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const
{
    return ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const
{
    return ordersLeft > 0;
}

bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const
{
    if (!isBusy())
    {
        if (hasOrdersLeft())
        {
            if
                 (getMaxDistance()>= order.getDistance())
                {
                    return true;
                }
        }
    }
    return false;

} // Signal if the volunteer is not busy, the order is within the maxDistance and have orders left

void LimitedDriverVolunteer::acceptOrder(const Order &order)
{
    if (canTakeOrder(order))
    {
        activeOrderId = order.getId();
        ordersLeft -= 1;
        setDistanceLeft(order.getDistance());
    }

} // Reset distanceLeft to maxDistance and decrease ordersLeft

string LimitedDriverVolunteer::toString() const
{
    return "LimitedDriver"; // to string is returning the type
}
void LimitedDriverVolunteer::step()
{
    if (isBusy())
    {
        if (decreaseDistanceLeft())
        {
            completedOrderId += 1;
            activeOrderId = -1;
            setDistanceLeft(-1); // he is with no delivery at the momemnt
            ordersLeft--;
        }
    }

} // Decrease distanceLeft by distancePerStep
//additional func
