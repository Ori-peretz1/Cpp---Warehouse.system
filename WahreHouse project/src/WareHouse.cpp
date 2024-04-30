#include "WareHouse.h"
#include "Volunteer.h"
#include "Order.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

// move assignment
WareHouse &WareHouse::operator=(WareHouse &&other)
{
    if (this != &other)
    {

        isOpen = other.isOpen;
        for (auto *v : actionsLog)
        {
            delete v;
            v = nullptr;
        }
        actionsLog.clear();
        for (auto *v : volunteers)
        {
            delete v;
            v = nullptr;
        }
        volunteers.clear();
        for (auto *v : pendingOrders)
        {
            delete v;
            v = nullptr;
        }
        pendingOrders.clear();
        for (auto *v : inProcessOrders)
        {
            delete v;
            v = nullptr;
        }
        inProcessOrders.clear();
        for (auto *v : completedOrders)
        {
            delete v;
            v = nullptr;
        }
        completedOrders.clear();
        for (auto *v : customers)
        {
            delete v;
            v = nullptr;
        }
        customers.clear();

        for (auto *v : other.actionsLog)
        {
            actionsLog.push_back(v);
        }
        other.actionsLog.clear();
        for (auto *v : other.volunteers)
        {
            volunteers.push_back(v);
        }
        other.volunteers.clear();
        for (auto *v : other.pendingOrders)
        {
            pendingOrders.push_back(v);
        }
        other.pendingOrders.clear();
        for (auto *v : other.inProcessOrders)
        {
            inProcessOrders.push_back(v);
        }
        other.inProcessOrders.clear();
        for (auto *v : other.completedOrders)
        {
            completedOrders.push_back(v);
        }
        other.completedOrders.clear();
        for (auto *v : other.customers)
        {
            customers.push_back(v);
        }
        other.customers.clear();

        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderId = other.orderId;
    }
    return *this;
}

// move constructor
WareHouse::WareHouse(WareHouse &&other) : isOpen(false), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), customerCounter(0), volunteerCounter(0), orderId(-1)
{
    isOpen = other.isOpen;

    for (auto *v : other.actionsLog)
    {
        actionsLog.push_back(v);
    }
    other.actionsLog.clear();
    for (auto *v : other.volunteers)
    {
        volunteers.push_back(v);
    }
    other.volunteers.clear();
    for (auto *v : other.pendingOrders)
    {
        pendingOrders.push_back(v);
    }
    other.pendingOrders.clear();
    for (auto *v : other.inProcessOrders)
    {
        inProcessOrders.push_back(v);
    }
    other.inProcessOrders.clear();
    for (auto *v : other.completedOrders)
    {
        completedOrders.push_back(v);
    }
    other.completedOrders.clear();
    for (auto *v : other.customers)
    {
        customers.push_back(v);
    }
    other.customers.clear();

    customerCounter = other.customerCounter;
    volunteerCounter = other.volunteerCounter;
    orderId = other.orderId;
}
// Destructor
WareHouse::~WareHouse()
{
    for (auto *v : actionsLog)
    {
        delete v;
        v = nullptr;
    }
    actionsLog.clear();
    for (auto *v : volunteers)
    {
        delete v;
        v = nullptr;
    }
    volunteers.clear();
    for (auto *v : pendingOrders)
    {
        delete v;
        v = nullptr;
    }
    pendingOrders.clear();
    for (auto *v : inProcessOrders)
    {
        delete v;
        v = nullptr;
    }
    inProcessOrders.clear();
    for (auto *v : completedOrders)
    {
        delete v;
        v = nullptr;
    }
    completedOrders.clear();
    for (auto *v : customers)
    {
        delete v;
        v = nullptr;
    }
    customers.clear();
}
// copy constructor
WareHouse::WareHouse(const WareHouse &other) : isOpen(other.isOpen), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), orderId(other.orderId)
{
    for (auto *v : other.actionsLog)
    {
        actionsLog.push_back(v->clone());
    }
    for (auto &v : other.volunteers)
    {
        volunteers.push_back(v->clone());
    }
    for (auto *v : other.pendingOrders)
    {
        pendingOrders.push_back(v->clone());
    }
    for (auto *v : other.inProcessOrders)
    {
        inProcessOrders.push_back(v->clone());
    }
    for (auto *v : other.completedOrders)
    {
        completedOrders.push_back(v->clone());
    }
    for (auto *v : other.customers)
    {
        customers.push_back(v->clone());
    }
}
// copy assignment
WareHouse &WareHouse::operator=(const WareHouse &other)
{
    if (this != &other)
    {
        isOpen = other.isOpen;
        for (auto *v : actionsLog)
        {
            delete v;
            v = nullptr;
        }
        actionsLog.clear();
        for (auto *v : volunteers)
        {
            delete v;
            v = nullptr;
        }
        volunteers.clear();
        for (auto *v : pendingOrders)
        {
            delete v;
            v = nullptr;
        }
        pendingOrders.clear();
        for (auto *v : inProcessOrders)
        {
            delete v;
            v = nullptr;
        }
        inProcessOrders.clear();
        for (auto *v : completedOrders)
        {
            delete v;
            v = nullptr;
        }
        completedOrders.clear();
        for (auto *v : customers)
        {
            delete v;
            v = nullptr;
        }
        customers.clear();

        for (auto *v : other.actionsLog)
        {
            actionsLog.push_back(v->clone());
        }
        for (auto *v : other.volunteers)
        {
            volunteers.push_back(v->clone());
        }
        for (auto *v : other.pendingOrders)
        {
            pendingOrders.push_back(v->clone());
        }
        for (auto *v : other.inProcessOrders)
        {
            inProcessOrders.push_back(v->clone());
        }
        for (auto *v : other.completedOrders)
        {
            completedOrders.push_back(v->clone());
        }
        for (auto *v : other.customers)
        {
            customers.push_back(v->clone());
        }

        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderId = other.orderId;
    }
    return *this;
}

WareHouse::WareHouse(const string &configFilePath) : isOpen(false), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), customerCounter(0), volunteerCounter(0), orderId(0)
{

    // Reference: https://stackoverflow.com/questions/3910326/c-read-file-line-by-line-then-split-each-line-using-the-delimiter
    ifstream file(configFilePath);
    string line;
    int VolunteerIDs = 0;
    int CustomerIDs = 0;

    while (getline(file, line))
    {

        {

            stringstream linestream(line);
            string data;
            string name;
            string type;
            int val1;
            int val2;
            int val3;

            getline(linestream, data, ' ');
            if (data == "volunteer" || data == "customer")
            {
                linestream >> name >> type >> val1 >> val2 >> val3;
                // cout << data + " " + name + " " + type + " " << val1 << " " << val2 << " " << val3 << endl;
                if (data == "volunteer" && type == "collector")
                {
                    Volunteer *v = new CollectorVolunteer(VolunteerIDs, name, val1);
                    VolunteerIDs++;
                    volunteers.push_back(v);
                }
                else if (data == "volunteer" && type == "limited_collector")
                {
                    Volunteer *v = new LimitedCollectorVolunteer(VolunteerIDs, name, val1, val2);
                    VolunteerIDs++;
                    volunteers.push_back(v);
                }
                else if (data == "volunteer" && type == "driver")
                {
                    Volunteer *v = new DriverVolunteer(VolunteerIDs, name, val1, val2);
                    VolunteerIDs++;
                    volunteers.push_back(v);
                }
                else if (data == "volunteer" && type == "limited_driver")
                {
                    Volunteer *v = new LimitedDriverVolunteer(VolunteerIDs, name, val1, val2, val3);
                    VolunteerIDs++;
                    volunteers.push_back(v);
                }
                else if (data == "customer" && type == "civilian")
                {
                    Customer *v = new CivilianCustomer(CustomerIDs, name, val1, val2);
                    CustomerIDs++;
                    customers.push_back(v);
                }
                else if (data == "customer" && type == "soldier")
                {
                    Customer *v = new SoldierCustomer(CustomerIDs, name, val1, val2);
                    CustomerIDs++;
                    customers.push_back(v);
                }
                else
                {
                    cerr << "Unknown type" << endl;
                }
            }
        }
    }
    customerCounter = CustomerIDs;
    volunteerCounter = VolunteerIDs;
}
void WareHouse::start()
{
    string input;
    cout << "WareHouse is open" << endl;
    isOpen = true;
    std::cout << "Choose a method from the following methods:" << endl;
    std::cout << "log || close || backup || restore || step || order || customer (customer type with first capital letter)" << std::endl;
    std::cout << "||orderStatus || customerStatus || volunteerStatus" << std::endl;
    while (isOpen)
    {

        getline(cin, input);

        if (input.substr(0, 3) == "log" && input.size() == 3)
        {
            PrintActionsLog *actionLog = new PrintActionsLog();
            actionLog->act(*this);
            addAction(actionLog);
        }
        else if (input.substr(0, 5) == "close" && input.size() == 5)
        {
            Close *close = new Close();
            close->act(*this);
            addAction(close);
        }
        else if (input.substr(0, 6) == "backup" && input.size() == 6)
        {
            BackupWareHouse *backup = new BackupWareHouse();
            backup->act(*this);
            addAction(backup);
        }
        else if (input.substr(0, 7) == "restore" && input.size() == 7)
        {
            RestoreWareHouse *restore = new RestoreWareHouse();
            restore->act(*this);
            addAction(restore);
        }
        else if (input.substr(0, 4) == "step" && input.size() >= 5 && input.at(4) == ' ')
        {
            std::stringstream ss;
            for (char c : input)
                if (std::isdigit(c))
                    ss << c;

            int stepNumber;
            ss >> stepNumber;
            SimulateStep *step = new SimulateStep(stepNumber);
            step->act(*this);
            addAction(step);
        }
        else if (input.substr(0, 11) == "orderStatus" && input.size() >= 12 && input.at(11) == ' ')
        {
            std::stringstream ss;
            for (char c : input)
                if (std::isdigit(c))
                    ss << c;

            int id;
            ss >> id;
            PrintOrderStatus *orderStatus = new PrintOrderStatus(id);
            orderStatus->act(*this);
            addAction(orderStatus);
        }
        else if (input.substr(0, 14) == "customerStatus" && input.size() >= 15 && input.at(14) == ' ')
        {
            std::stringstream ss;
            for (char c : input)
                if (std::isdigit(c))
                    ss << c;

            int customerId;
            ss >> customerId;
            PrintCustomerStatus *customerStatus = new PrintCustomerStatus(customerId);
            customerStatus->act(*this);
            addAction(customerStatus);
        }
        else if (input.substr(0, 5) == "order" && input.size() >= 6 && input.at(5) == ' ')
        {

            std::stringstream ss;
            for (char c : input)
                if (std::isdigit(c))
                    ss << c;

            int id;
            ss >> id;
            AddOrder *addOrder = new AddOrder(id);
            addOrder->act(*this);
            addAction(addOrder);
        }
        else if (input.substr(0, 8) == "customer" && input.size() >= 9 && input.at(8) == ' ')
        {
            istringstream iss(input);
            string keyword, customerName, customerType;
            int distance, maxOrders;
            iss >> keyword >> customerName >> customerType >> distance >> maxOrders;
            AddCustomer *customer = new AddCustomer(customerName, customerType, distance, maxOrders);
            customer->act(*this);
            addAction(customer);
        }
        else if (input.substr(0, 15) == "volunteerStatus" && input.size() >= 16 && input.at(15) == ' ')
        {
            std::stringstream ss;
            for (char c : input)
                if (std::isdigit(c))
                    ss << c;

            int volunId;
            ss >> volunId;
            PrintVolunteerStatus *volunStatus = new PrintVolunteerStatus(volunId);
            volunStatus->act(*this);
            addAction(volunStatus);
        }
        else
        {

            cout << "There is no method like this" << endl;
        }
    }
}

const vector<BaseAction *> &WareHouse::getActions() const
{

    // --- - - - - - - - - - - - - - - - - -

    return actionsLog;
}

void WareHouse::addOrder(Order *order)
{
    pendingOrders.push_back(order);
    orderId = orderId + 1;
}
void WareHouse::addCustomer(Customer *customer)
{
    customers.push_back(customer);
    customerCounter++;
} // additional func

void WareHouse::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}
int WareHouse::getOrderId()
{
    return orderId;
}

Customer &WareHouse::getCustomer(int customerId) const
{
    for (unsigned int i = 0; i < customers.size(); i++)
    {
        if (customers.at(i)->getId() == customerId)
        {
            return *customers[i];
        }
    }
    return cFictive;
}

Volunteer &WareHouse::getVolunteer(int volunteerId) const
{
    for (unsigned int i = 0; i < volunteers.size(); i++)
    {
        if (volunteers.at(i)->getId() == volunteerId)
        {

            return *volunteers[i];
        }
    }

    return vFictive;
}

Order &WareHouse::getOrder(int orderId) const
{
    for (unsigned int i = 0; i < pendingOrders.size(); i++)
    {
        if (pendingOrders[i]->getId() == orderId)
        {
            return *pendingOrders[i];
        }
    }
    for (unsigned int i = 0; i < inProcessOrders.size(); i++)
    {
        if (inProcessOrders[i]->getId() == orderId)
        {
            return *inProcessOrders[i];
        }
    }
    for (unsigned int i = 0; i < completedOrders.size(); i++)
    {
        if (completedOrders[i]->getId() == orderId)
        {
            return *completedOrders[i];
        }
    }

    return oFictive;
}

void WareHouse::close()
{
    isOpen = false;
}

void WareHouse::open()
{
    isOpen = true;
}

vector<Order *> &WareHouse::getPendingOrders()
{
    return pendingOrders;
} // additional func

vector<Order *> &WareHouse::getInProcessOrders()
{
    return inProcessOrders;
} // additional func

vector<Volunteer *> &WareHouse::getVolunteers()
{
    return volunteers;
} // additional func
vector<Order *> &WareHouse::getCompletedOrders()
{
    return completedOrders; // additional func
}

vector<Customer *> &WareHouse::getCustomers()
{
    return customers;
}

int WareHouse::getCustomerCounter()
{
    return customerCounter;
} // additional func

void WareHouse::setCustomerCounterPlus1()
{
    customerCounter++;
} // additional func
int WareHouse::getCustomerId(Customer *cus)
{
    int j = -1;
    for (unsigned int i = 0; (i < customers.size()) && (j == -1); i++)
    {
        if (cus == customers[i])
        {
            j = i;
        }
    }
    return j;

} // add func

void WareHouse::deleteVolunteer(int volunteerId)
{
    for (unsigned int i = 0; i < volunteers.size(); i++)
    {
        if (volunteers[i]->getId() == volunteerId)
        {
            volunteers.erase(volunteers.begin() + i);
        }
    }
}

void WareHouse::erasePendingOrder(Order *o)
{
    bool hasFound = false;
    for (unsigned int i = 0; (i < pendingOrders.size()) && (!hasFound); i++)
    {
        if (pendingOrders[i] == o)
        {
            (pendingOrders).erase(pendingOrders.begin() + i);
            hasFound = true;
        }
    }
}

void WareHouse::eraseProcessOrder(Order *o)
{
    bool hasFound = false;
    for (unsigned int i = 0; (i < inProcessOrders.size()) && (!hasFound); i++)
    {
        if (inProcessOrders[i] == o)
        {
            (inProcessOrders).erase(inProcessOrders.begin() + i);
            hasFound = true;
        }
    }
} // additional func
void WareHouse::deleteLastBackup(WareHouse *&backup)
{
    *this = *backup; // using move assignment operator
    delete backup;
    backup = nullptr;
}

Order WareHouse::oFictive(-1, -1, -1);
CivilianCustomer WareHouse::cFictive(-1, "", -1, -1);
CollectorVolunteer WareHouse::vFictive(-1, "", -1);
