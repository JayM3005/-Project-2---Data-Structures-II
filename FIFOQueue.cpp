#include "FIFOQueue.hpp"

FIFOQueue::FIFOQueue() {
    head = nullptr;
    tail = nullptr;
    count = 0;
}

void FIFOQueue::enqueue(double arrivalTime) {
    // Creates the new customer
    Customer* newCustomer = new Customer;
    newCustomer->arrivalTime = arrivalTime;
    newCustomer->next = nullptr;

    // If line is empty, head and tail are the same
    if (isEmpty()) {
        head = newCustomer;
        tail = newCustomer;
    } else {
        // Adds to the end
        tail->next = newCustomer;
        tail = newCustomer;
    }
    count++;
}

double FIFOQueue::dequeue() {
    if (isEmpty()) {
        return -1.0; 
    }

    // Grabs the data we need to return
    double time = head->arrivalTime;

    // Moves the head pointer
    Customer* temp = head;
    head = head->next;

    // If the line is now empty, fixes the tail
    if (head == nullptr) {
        tail = nullptr;
    }

    // Clean up memory
    delete temp; 
    count--;

    return time;
}

bool FIFOQueue::isEmpty() {
    return head == nullptr;
}