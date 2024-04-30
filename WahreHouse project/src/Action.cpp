#include <string>
#include <vector>
#include <iostream>
#include "Action.h"
using std::string;
using std::vector;
extern WareHouse *backup;

BaseAction::BaseAction() : errorMsg(), status(ActionStatus::COMPLETED)
{
}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
    
}
void BaseAction::setStatus(ActionStatus status)
{
    this->status = status; // additional func
}
void BaseAction::setErrorMsg(string str)
{
    errorMsg = str;
} // add func

void BaseAction::error(string errorMsg)
{
    this->errorMsg = errorMsg;
    status = ActionStatus::ERROR;
}

string BaseAction::getErrorMsg() const
{
    return errorMsg;
}

//------------------------------------------------------------------------------------------------------

SimulateStep::SimulateStep(int numOfSteps) : BaseAction(), numOfSteps(numOfSteps)
{
}

void SimulateStep::act(WareHouse &wareHouse)
{

    for (int i = numOfSteps; i > 0; i--)
    {
        for (unsigned w = 0; (w < wareHouse.getPendingOrders().size()); w++)
        {
            Order* itemO = wareHouse.getPendingOrders()[w];
            if (itemO->getStatus() == OrderStatus::PENDING)
            {
                bool found = false;
                for (unsigned int k=0; (k < wareHouse.getVolunteers().size()) && (!found); k++ )
                {
                    Volunteer *itemV =wareHouse.getVolunteers()[k];

                    
                    if (((itemV->toString() == "Collector") || (itemV->toString() == "LimitedCollector")) && (itemV->canTakeOrder(*itemO)))
                    {
                        itemV->acceptOrder(*itemO);
                        itemO->setStatus(OrderStatus::COLLECTING);
                        itemO->setCollectorId(itemV->getId());
                        wareHouse.erasePendingOrder(itemO);
                        w--; // if we erase some itemO from the vector, the vector decrease -1 him size.
                        wareHouse.getInProcessOrders().push_back(itemO);
                        found = true;
                    }
                }
            }
            else if (itemO->getStatus() == OrderStatus::COLLECTING)
            { // pending between collector and driver
                bool found = false;
                for (unsigned int n=0; (n < wareHouse.getVolunteers().size()) && (!found); n++)
                {
                    Volunteer *itemV = wareHouse.getVolunteers()[n];
                    if ((itemV->toString() == "Driver" || itemV->toString() == "LimitedDriver") && itemV->canTakeOrder(*itemO))
                    {
                        itemV->acceptOrder(*itemO);
                        itemO->setStatus(OrderStatus::DELIVERING);
                        itemO->setDriverId(itemV->getId());
                        wareHouse.erasePendingOrder(itemO);
                        w--; // if we erase some itemO from the vector, the vector decrease -1 him size.
                        wareHouse.getInProcessOrders().push_back(itemO);
                        found = true;
                    }
                }
            }
        }
        for (Volunteer *itemV : wareHouse.getVolunteers())
        {
            
            int numberTheLastOrder = itemV->getActiveOrderId(); // available even after setting orders
            itemV->step();
            if (!(itemV->isBusy()) && (numberTheLastOrder != -1))
            {
                bool found = false;
                for (unsigned z=0; (z< wareHouse.getInProcessOrders().size()) && !found; z++)
                {   

                    Order* O = wareHouse.getInProcessOrders()[z];
                    if (O->getId() == numberTheLastOrder)
                    {
                        if (O->getStatus() == OrderStatus::COLLECTING)
                        {
                            // Stay Collecting status.
                            wareHouse.eraseProcessOrder(O);
                            wareHouse.getPendingOrders().push_back(O);
                            found = true;
                        }
                        else if (O->getStatus() == OrderStatus::DELIVERING)
                        {
                            O->setStatus(OrderStatus::COMPLETED);
                            wareHouse.eraseProcessOrder(O);
                            wareHouse.getCompletedOrders().push_back(O);
                            found = true;
                        }
                    }
                }
                if (itemV->toString() == "LimitedCollector" && !itemV->hasOrdersLeft())
                {
                    wareHouse.deleteVolunteer(itemV->getId());
                    delete (itemV);
                }
                else if (itemV->toString() == "LimitedDriver" && !itemV->hasOrdersLeft())
                {
                    wareHouse.deleteVolunteer(itemV->getId());
                    delete (itemV);
                }
            }
        }
    }
    complete();
}

std::string SimulateStep::toString() const
{
    return "SimulateStep " + std::to_string(numOfSteps) + " "; // we chose to print the num of steps bacause there is no required way
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

//------------------------------------------------------------------------------------------------------

AddOrder::AddOrder(int id) : BaseAction(), customerId(id)
{
}

void AddOrder::act(WareHouse &wareHouse)
{
    int i = -1;
    int distance = -1;
    for (unsigned int j = 0; (j < wareHouse.getCustomers().size()) && (i == -1); j++)
    {
        if ((((wareHouse.getCustomers()[j])->getId() == customerId)) && (wareHouse.getCustomers()[j]->canMakeOrder()))
        {
            distance = (wareHouse.getCustomers()[j])->getCustomerDistance();
            i = j;
        }
    }
    if (i != -1)
    {
        Order *newO = new Order(wareHouse.getOrderId(), customerId, distance);
        wareHouse.getCustomers()[i]->addOrder(wareHouse.getOrderId());
        wareHouse.addOrder(newO);
        complete();
    }
    else
    {
        error(("Cannot place this order")); // update the errmsg field
        std::cout << "Error: Cannot place this order" << endl;
    }
}

AddOrder *AddOrder::clone() const
{
    return new AddOrder(*this);
}

string AddOrder::toString() const
{

    return "order " + std::to_string(customerId) + " ";
}

//------------------------------------------------------------------------------------------------------

AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders) : BaseAction(), customerName(customerName),
                                                                                                                customerType((customerType == "Soldier") ? CustomerType::Soldier : CustomerType::Civilian), distance(distance), maxOrders(maxOrders) {}

void AddCustomer::act(WareHouse &wareHouse)
{
    

    if (customerType == CustomerType::Soldier)
    {
        Customer *newCus = new SoldierCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
        wareHouse.addCustomer(newCus);
    }
    else
    {
        Customer *newCus = new CivilianCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
        wareHouse.addCustomer(newCus);
    }
    complete();

}

AddCustomer *AddCustomer::clone() const
{
    return new AddCustomer(*this);
}

string AddCustomer::toString() const
{
    
    if (customerType == CustomerType::Soldier)
    {

        return "Added: customer " + customerName + " Soldier " + std::to_string(distance) + " " + std::to_string(maxOrders) + " - ";
    }
    else
    {
        return "Added: customer " + customerName + " Civilian " + std::to_string(distance) +  " " + std::to_string(maxOrders) + " - ";
    }


}

//------------------------------------------------------------------------------------------------------

PrintOrderStatus::PrintOrderStatus(int id) : BaseAction(), orderId(id)
{
}

void PrintOrderStatus::act(WareHouse &wareHouse)
{
    if(wareHouse.getOrder(orderId).getCustomerId() != -1) {
        if ((orderId <= wareHouse.getOrderId()) && (orderId >= 0)) // our ivariant count from zero to curr order No', so any order that ever placed has smaller number than orderId field im wareHouse
    {
        std::cout << "OrderId: " << orderId << std::endl;
        std::cout << "OrderStatus: " << wareHouse.getOrder(orderId).getStringOrderStatus() << std::endl;
        std::cout << "CustomerId: " << std::to_string(wareHouse.getOrder(orderId).getCustomerId())<<std::endl;
        if (wareHouse.getOrder(orderId).getCollectorId() != -1)
        {
            std::cout << "Collector: " << std::to_string(wareHouse.getOrder(orderId).getCollectorId()) << std::endl;
            complete();
        }
        else
        {
            std::cout << "Collector: " << " None" << std::endl;
                      
            complete();
        }
        if (wareHouse.getOrder(orderId).getDriverId() != -1)
        {
            std::cout << "Driver: " << std::to_string(wareHouse.getOrder(orderId).getDriverId()) <<std::endl;
            complete();
        }
        else
        {
            std::cout << "Driver: " << " None" << std::endl;
            complete();
        }
    }
    }
    
    else
    {
        error("Order doesn't exist");
        std::cout << "Error: Order doesn't exist" << std::endl;
    }
}
PrintOrderStatus *PrintOrderStatus::clone() const
{
    return new PrintOrderStatus(*this);
}
string PrintOrderStatus::toString() const
{

    return "orderStatus " + std::to_string(orderId) + " ";
}

//------------------------------------------------------------------------------------------------------

PrintCustomerStatus::PrintCustomerStatus(int customerId) : BaseAction(), customerId(customerId)
{
}
void PrintCustomerStatus::act(WareHouse &wareHouse)
{
    if (wareHouse.getCustomer(customerId).getId() >= 0)
    { // he is exists
        std::cout << "CustomerId: " << std::to_string(customerId) << std::endl;
        for (unsigned int i = 0; i < ((wareHouse.getCustomer(customerId)).getOrdersids()).size(); i++)
        {
            int currentOrder = (wareHouse.getCustomer(customerId)).getOrdersids()[i];
            std::cout << "OrderId: " << std::to_string(currentOrder) << std::endl;
            wareHouse.getOrder(currentOrder);
            std::cout << "OrderStatus: " << wareHouse.getOrder(currentOrder).getStringOrderStatus() << std::endl;
        }
        std::cout << "numOfOrdersLeft: " << std::to_string((wareHouse.getCustomer(customerId)).getNumOfOrdersLeft()) << std::endl;
        complete();
    }
    else
    {

        error("Customer doesn't exist");
        std::cout << "Error: Customer doesn't exist" << std::endl;
    }
}

PrintCustomerStatus *PrintCustomerStatus::clone() const
{
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const
{
    return "customerStatus " + std::to_string(customerId);
}

//------------------------------------------------------------------------------------------------------

PrintVolunteerStatus::PrintVolunteerStatus(int id) : BaseAction(), volunteerId(id)
{
}
void PrintVolunteerStatus::act(WareHouse &wareHouse)
{
    int i = -1;
    for (unsigned int j = 0; (j < wareHouse.getVolunteers().size()) && (i == -1); j++)
    {
        if (wareHouse.getVolunteers()[j]->getId() == volunteerId)
        {
            i = 0; // we found!
        }
    }
    if (i == 0) // we found the voulunteer
    {
        std::cout << "VolunteerID: " << std::to_string(volunteerId) << std::endl;
        if (wareHouse.getVolunteer(volunteerId).isBusy())
        {
            std::cout << "isBusy: True" << std::endl;
            int activeOrder = wareHouse.getVolunteer(volunteerId).getActiveOrderId();
            std::cout << "OrderId: " << std::to_string(activeOrder) << std::endl;
            if (wareHouse.getVolunteer(volunteerId).toString() == "Collector")
            {
                CollectorVolunteer &vol = dynamic_cast<CollectorVolunteer &>(wareHouse.getVolunteer(volunteerId));
                std::cout << "TimeLeft: " << std::to_string((vol.getTimeLeft())) << std::endl;
                std::cout << "OrderLeft: No Limit" << std::endl;
            }
            else if (wareHouse.getVolunteer(volunteerId).toString() == "LimitedCollector")
            {
                LimitedCollectorVolunteer &vol = dynamic_cast<LimitedCollectorVolunteer &>(wareHouse.getVolunteer(volunteerId));
                std::cout << "TimeLeft: " << std::to_string((vol.getTimeLeft())) << std::endl;
                std::cout << "OrderLeft: " << std::to_string(vol.getNumOrdersLeft()) << std::endl;
            }
            else if (wareHouse.getVolunteer(volunteerId).toString() == "Driver")
            {
                DriverVolunteer &vol = dynamic_cast<DriverVolunteer &>(wareHouse.getVolunteer(volunteerId));
                std::cout << "DistanceLeft: " << std::to_string(vol.getDistanceLeft()) << std::endl;
                std::cout << "OrderLeft: No Limit" << std::endl;
            }
            else if (wareHouse.getVolunteer(volunteerId).toString() == "LimitedDriver")
            {
                LimitedDriverVolunteer &vol = dynamic_cast<LimitedDriverVolunteer &>(wareHouse.getVolunteer(volunteerId));
                std::cout << "DistanceLeft: " << std::to_string(vol.getDistanceLeft()) << std::endl;
                std::cout << "OrderLeft: " << std::to_string(vol.getNumOrdersLeft()) << std::endl;
            }
            complete();
        }
        else
        {
            std::cout << "isBusy: False" << std::endl;
            std::cout << "OrderId: None" << std::endl;
            if (wareHouse.getVolunteer(volunteerId).toString() == "Collector")
            {
                std::cout << "TimeLeft: None" << std::endl;
                std::cout << "OrderLeft: No Limit" << std::endl;
            }
            else if (wareHouse.getVolunteer(volunteerId).toString() == "LimitedCollector")
            {
                LimitedCollectorVolunteer &vol = dynamic_cast<LimitedCollectorVolunteer &>(wareHouse.getVolunteer(volunteerId));
                std::cout << "TimeLeft: None" << std::endl;

                std::cout << "OrderLeft: " << std::to_string(vol.getNumOrdersLeft()) << std::endl;
            }
            else if (wareHouse.getVolunteer(volunteerId).toString() == "Driver")
            {
                std::cout << "DistanceLeft: None" << std::endl;

                std::cout << "OrderLeft: No Limit" << std::endl;
            }
            else if (wareHouse.getVolunteer(volunteerId).toString() == "LimitedDriver")
            {
                LimitedDriverVolunteer &vol = dynamic_cast<LimitedDriverVolunteer &>(wareHouse.getVolunteer(volunteerId));
                std::cout << "DistanceLeft: None" << std::endl;

                std::cout << "OrderLeft: " << std::to_string(vol.getNumOrdersLeft()) << std::endl;
            }
            complete();
        }
    }
    else
    {
        error("Volunteer doesn't exist");
        std::cout << "Error: Volunteer doesn't exist" << std::endl;
    }
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const
{
    return new PrintVolunteerStatus(*this);
}
string PrintVolunteerStatus::toString() const
{
    return "volunteerStatus " + std::to_string(volunteerId) + " ";
}

//------------------------------------------------------------------------------------------------------

PrintActionsLog::PrintActionsLog() : BaseAction()
{
}
void PrintActionsLog::act(WareHouse &wareHouse)
{
    for (BaseAction *action : wareHouse.getActions())
    {
        string status = "";
        if (action != this)
        {
            if (action->getStatus() == ActionStatus::COMPLETED)
            {
                status = "COMPLETED ";
            }
            else
            {
                status = "ERROR ";
            }
            std::cout << action->toString() + " " << status << std::endl;
        }
    }
    complete();
}
PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}
string PrintActionsLog::toString() const
{

    return "log ";
}

//------------------------------------------------------------------------------------------------------

Close::Close() : BaseAction()
{
}
void Close::act(WareHouse &wareHouse)
{
    for (Order *o : wareHouse.getPendingOrders())
    {
        std::cout << "OrderId: " << o->getId() << "," << " CustomerId: " << o->getCustomerId() << "," << " OrderStatus: " << o->getStringOrderStatus() << std::endl;
    }
    for (Order *o : wareHouse.getInProcessOrders())
    {
        std::cout << "OrderId: " << o->getId() << "," << " CustomerId: " << o->getCustomerId() << "," << " OrderStatus: " << o->getStringOrderStatus() << std::endl;
    }
    for (Order *o : wareHouse.getCompletedOrders())
    {
        std::cout << "OrderId: " << o->getId() << "," << " CustomerId: " << o->getCustomerId() << "," << " OrderStatus: " << o->getStringOrderStatus() << std::endl;
    }
    complete();
    wareHouse.close(); // closing the wareHouse
}
string Close::toString() const
{
    return " wareHouse Is Close!";
}
Close *Close::clone() const
{
    return new Close(*this);
}

//------------------------------------------------------------------------------------------------------

BackupWareHouse::BackupWareHouse() : BaseAction()
{
}
void BackupWareHouse::act(WareHouse &wareHouse)
{
    if (backup != nullptr)
    {
        delete backup;
    }
    backup = new WareHouse(wareHouse);
    
    complete(); // using copy constructor of warehouse
}
BackupWareHouse *BackupWareHouse::clone() const
{
    return new BackupWareHouse(*this);
}

string BackupWareHouse::toString() const
{
    return "backup";
}

//------------------------------------------------------------------------------------------------------

RestoreWareHouse::RestoreWareHouse() : BaseAction()
{
}
void RestoreWareHouse::act(WareHouse &wareHouse)

{
    if (backup == nullptr)
    {
        std::cout << "Error: No backup available" << std::endl;
        this->setStatus(ActionStatus::ERROR);
    }
    else
    {
        wareHouse.deleteLastBackup(backup);
        complete();
    }
    
}
RestoreWareHouse *RestoreWareHouse::clone() const
{
    return new RestoreWareHouse(*this);
}
string RestoreWareHouse::toString() const
{
    return "restore";
}