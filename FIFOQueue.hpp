#ifndef FIFOQUEUE_HPP
#define FIFOQUEUE_HPP

#include <iostream>

using namespace std;

// Represents a Customer standing in line
struct Customer {
    double arrivalTime; 
    Customer* next;
};

class FIFOQueue {
private:
    Customer* head; // Front of the line 
    Customer* tail; // Back of the line 
    int count;     

public:
    FIFOQueue();
    // Destructor
    ~FIFOQueue(); 
    
    // Adds a customer to the back
    void enqueue(double arrivalTime);
    
    // Removes a customer from the front and return their arrival time
    double dequeue();
    
    // Checks if line is empty
    bool isEmpty();
};

#endif