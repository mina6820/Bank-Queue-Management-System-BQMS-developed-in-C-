#pragma once
#include <iostream>
using namespace std;

enum CustomerType {
	Regular,
	VIP
};

class Customer
{
public:
	int ID;
	int arrival_time;
	int waiting_time;
	int service_time;
	int leaving_time;
	int serviced_teller;
	CustomerType type;

	Customer(int id, int arrival, int service, CustomerType customerType)
	{
		ID = id;
		arrival_time = arrival;
		service_time = service;
		waiting_time = 0;
		serviced_teller = 0;
		type = customerType;
	}

};
