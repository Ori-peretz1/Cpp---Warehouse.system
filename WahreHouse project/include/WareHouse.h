#pragma once
#include <string>
#include <vector>
using namespace std;

#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"
#include "Action.h"
// Warehouse responsible for Volunteers, Customers and Actions.
class BaseAction;

class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        const vector<BaseAction*> &getActions() const;
        vector<Order*>& getPendingOrders();        // additional func
        vector<Order*>& getInProcessOrders();   
        vector<Order*>& getCompletedOrders();      // additional func
        vector<Volunteer*>& getVolunteers();     // additional func
        vector<Customer*>& getCustomers(); 
        int getCustomerId (Customer* cus);       // additional func
        int getCustomerCounter(); // additional func
        void setCustomerCounterPlus1(); // additional func
        void addOrder(Order* order);
        void addCustomer(Customer* customer); // additional func
        int getOrderId (); // additional func
        void addAction(BaseAction* action);
        void deleteLastBackup (WareHouse*& lastBackup);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        void close();
        void open();
        //rule of 5
        // move assignment
        WareHouse& operator=(WareHouse&& others);
        // move constructor
        WareHouse(WareHouse&& other);
        //Destructor
        ~WareHouse();
        // copy constructor
        WareHouse(const WareHouse& other);
        // copy assignment
        WareHouse& operator=(const WareHouse& other);
        //added functions
        void deleteVolunteer(int volunteerId);
        void erasePendingOrder(Order* o); //additional func
        void eraseProcessOrder(Order* o); //additional func

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderId; // My Addition
        static CivilianCustomer cFictive; //additional field
        static CollectorVolunteer vFictive ;
        static Order oFictive;
};