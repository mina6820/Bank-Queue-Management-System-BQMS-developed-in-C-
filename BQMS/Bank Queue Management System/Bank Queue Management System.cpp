#include <iostream>
#include "teller.h"
#include "CustomerIOProcessor.h"
#include "Customer.h"
#include "Bank.h"
using namespace std;

int main()
{
    CustomerIOProcessor c1;
    c1.read_file("customer_data.txt");
    c1.output();
    

    return 0;
}
