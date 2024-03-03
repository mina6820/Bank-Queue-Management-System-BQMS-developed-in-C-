#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <queue> // Change from <deque> to <queue>
#include <vector>
#include <iomanip>
#include "Bank.h"
#include "Customer.h"

using namespace std;

class CustomerIOProcessor {
private:
    struct CompareCustomers {
        bool operator()(const Customer& a, const Customer& b) const {
            if (a.arrival_time == b.arrival_time) {
                return b.type == CustomerType::VIP; 
            }
            return a.arrival_time > b.arrival_time; 
        }
    };

    priority_queue<Customer, vector<Customer>, CompareCustomers> customersPriorityQueue;
    Bank bank;
public:
    CustomerIOProcessor() {}

    void read_file(const string& filename)
    {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            // Extracting data from the line
            int customer_id, arrival_time, service_time;
            char colon;
            string customer_str, arrival_str, service_str, type_str;
            CustomerType type = CustomerType::Regular; // Default type is Regular
            istringstream iss(line);

            if (iss >> customer_str >> customer_id >> colon >> arrival_str >> arrival_time >> service_str >> service_time >> type_str) {
                if (customer_str == "customer" && arrival_str == "arrival" && service_str == "service" && type_str == "type" && colon == ':') {

                    size_t pos = line.find("type");
                    if (pos != string::npos) {
                        type_str = line.substr(pos + 5); // Skip "type "

                        // Determine customer type
                        if (type_str == "regular") {
                            type = CustomerType::Regular;
                        }
                        else if (type_str == "vip") {
                            type = CustomerType::VIP;
                        }
                    }

                    // Store the customer in the priority queue
                    customersPriorityQueue.emplace(customer_id, arrival_time, service_time, type);
                }
                else {
                    cerr << "Error parsing line: " << line << endl;
                }
            }
            else {
                cerr << "Error parsing line: " << line << endl;
            }
        }

        // Initialize the bank with the priority queue
        // Transfer elements from priority queue to deque while maintaining order
        deque<Customer> customersDeque;
        while (!customersPriorityQueue.empty()) {
            customersDeque.push_back(customersPriorityQueue.top());
            customersPriorityQueue.pop();
        }
        bank = Bank(customersDeque);
        bank.runSimulation();
        file.close();
    }


 
    void output() {
        ofstream outputFile("output.txt");
        deque<Customer> updatedDeque = bank.getUpdatedCustomerDeque(); // Get the updated customer deque

        // Output headers
        outputFile << setw(20) << left << "Customer"
            << setw(20) << left << "Arrival Time"
            << setw(20) << left << "Service Time"
            << setw(20) << left << "Waiting Time"
            << setw(25) << left << "Serviced by Teller"
            << setw(20) << left << "Leaving Time"
            << setw(150) << left << "\t    Type" << endl;
        outputFile << string(150, '-') << endl;

        while (!updatedDeque.empty()) {
            Customer customer = updatedDeque.front();
            outputFile << setw(23) << left << customer.ID
                << setw(22) << left << customer.arrival_time
                << setw(22) << left << customer.service_time
                << setw(22) << left << customer.waiting_time
                << setw(23) << left << customer.serviced_teller
                << setw(20) << left << customer.leaving_time
                << setw(125) << left << (customer.type == CustomerType::Regular ? "Regular" : "VIP") << endl;
            outputFile << string(150, '-') << endl;
            updatedDeque.pop_front();
        }

        // Output information about tellers
        vector<teller> tellers = bank.getTellers();
        for (const teller& t : tellers) {
            outputFile << endl <<"Teller " << t.teller_number << " served " << t.total_serveced_customers << " customers and worked for " << t.total_time_working << " units of time." << endl << endl;
        }

        // Output average waiting time
        outputFile << "Average Waiting Time For Customers: " << bank.getAvgWaitingTime() << endl;

        outputFile.close();
    }

};

