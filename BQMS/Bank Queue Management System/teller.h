#pragma once
#include <iostream>
using namespace std;

struct teller
{
    bool Available;
    int teller_number;
    int leaving_time;

    int total_time_working;
    int total_serveced_customers;

    teller()
    {
        Available = true;
        total_time_working = 0;
        total_serveced_customers = 0;
    }
};

