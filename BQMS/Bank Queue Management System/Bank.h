#pragma once
#include <iostream>
#include <deque>
#include <vector>
#include "teller.h"
#include "CustomerIOProcessor.h"
#include "Customer.h"
using namespace std;

class Bank {
private:
    deque<Customer> customersDeque;
    deque<Customer> finalCustomerDeque;

    vector<teller> tellers;

    int total_waiting_time = 0;
    int customers_count;

public:
    Bank() {}
    Bank(deque<Customer>& customersDeque) : customersDeque(customersDeque) {

        // Initialize The Tellers
        for (int i = 0; i < 3; ++i) {
            teller new_teller;
            new_teller.teller_number = i + 1;
            new_teller.total_serveced_customers = 0;
            new_teller.total_time_working = 0;
            tellers.push_back(new_teller);
        }
    }


    void runSimulation() {
        customers_count = customersDeque.size();

        while (!customersDeque.empty()) {
            Customer& current_customer = customersDeque.front();

            // Check if there is an available teller
            teller* available_teller = findAvailableTeller();

            // Check if the current customer is VIP
            if (current_customer.type == CustomerType::VIP && current_customer.serviced_teller != 0)
            {
                // Skip VIP customers for now, they will be processed separately
                customersDeque.pop_front();
                continue;
            }
            // if there is an empty teller
            if (available_teller != nullptr) {

                current_customer.waiting_time = 0;
                current_customer.leaving_time = current_customer.arrival_time + current_customer.service_time;
                current_customer.serviced_teller = available_teller->teller_number;

                available_teller->Available = false;
                available_teller->leaving_time = current_customer.leaving_time;
                available_teller->total_serveced_customers++;
                available_teller->total_time_working += current_customer.service_time;

                finalCustomerDeque.push_back(current_customer);
                customersDeque.pop_front();
            }

            // if All tellers are occupied
            else {

                int min_leaving_time = tellers[0].leaving_time;
                int chosen_teller_index = 0;

                // Find the teller with the minimum leaving time
                for (int i = 1; i < tellers.size(); ++i) {
                    if (min_leaving_time > tellers[i].leaving_time) {
                        min_leaving_time = tellers[i].leaving_time;
                        chosen_teller_index = i;
                    }
                }
                // Assign the customer to the teller with the minimum leaving time
                teller& chosen_teller = tellers[chosen_teller_index];

                // Get the index of the VIP customer
                int vipCustomerIndex = searchForVip(chosen_teller_index);

                // if there is a vip customer in the queue
                if (vipCustomerIndex != -1)
                {
                    Customer& vipCustomer = customersDeque[vipCustomerIndex];
                    //if (chosen_teller.leaving_time >= vipCustomer.arrival_time) {
                    //    vipCustomer.waiting_time = chosen_teller.leaving_time - vipCustomer.arrival_time;
                    //}
                    //else {
                    //    vipCustomer.waiting_time = 0; // The VIP customer arrives after the chosen teller's leaving time
                    //}
                    vipCustomer.waiting_time = max(0,chosen_teller.leaving_time - vipCustomer.arrival_time);
                    total_waiting_time += vipCustomer.waiting_time;
                    vipCustomer.leaving_time = vipCustomer.arrival_time + vipCustomer.waiting_time + vipCustomer.service_time;
                    vipCustomer.serviced_teller = chosen_teller.teller_number;
                    chosen_teller.Available = false;
                    chosen_teller.leaving_time = vipCustomer.leaving_time;

                    chosen_teller.total_serveced_customers++;
                    chosen_teller.total_time_working += vipCustomer.service_time;

                    finalCustomerDeque.push_back(vipCustomer);

                    customersDeque.erase(customersDeque.begin() + vipCustomerIndex); // Erase the VIP customer from deque
                }
                else
                {

                    current_customer.waiting_time = max(0,chosen_teller.leaving_time - current_customer.arrival_time);

                    total_waiting_time += current_customer.waiting_time;
                    current_customer.leaving_time = current_customer.arrival_time + current_customer.waiting_time + current_customer.service_time;
                    current_customer.serviced_teller = chosen_teller.teller_number;
                    chosen_teller.Available = false;
                    chosen_teller.leaving_time = current_customer.leaving_time;

                    chosen_teller.total_serveced_customers++;
                    chosen_teller.total_time_working += current_customer.service_time;

                    finalCustomerDeque.push_back(current_customer);
                    customersDeque.pop_front();
                }
            }
        }
    }

    // Function to find an available teller
    teller* findAvailableTeller() {
        for (int i = 0; i < tellers.size(); ++i) {
            if (tellers[i].Available)
                return &tellers[i];
        }
        return nullptr;   // No available teller found
    }

    int searchForVip(int chosen_teller_index) {
        int min_leaving_time = tellers[chosen_teller_index].leaving_time;

        for (int i = 0; i < customersDeque.size(); ++i) {
            if (customersDeque[i].type == CustomerType::VIP && customersDeque[i].arrival_time <= min_leaving_time) {
                // Return the index of the VIP customer
                return i;
            }
        }

        // If no VIP customer found
        return -1;
    }

    // Function to get the updated customer queue to the IOProcessor class
    deque<Customer> getUpdatedCustomerDeque() {
        return finalCustomerDeque;
    }

    vector<teller> getTellers()
    {
        return tellers;
    }

    int getAvgWaitingTime()
    {
        return total_waiting_time / customers_count;
    }
};


