#ifndef PRIORITYQUEUE_HPP
#define PRIORITYQUEUE_HPP
#include <iostream>
#include <vector>

using namespace std;

// Represents one specific moment in time to process
struct Event {
    double time;        
    int type;           
    
    bool operator>(const Event& other) const {
        return time > other.time;
    }
    bool operator<(const Event& other) const {
        return time < other.time;
    }
};

// The Min Heap Class
class PriorityQueue {
private:
    vector<Event> heap;
    
    // Fixes the heap order
    void percolateUp(int index);
    void percolateDown(int index);

public:
    PriorityQueue();

    // Adds a new event to the pile
    void insert(const Event& e);

    // Removes and returns the earliest event, the root
    Event deleteMin();

    // Checks if empty
    bool isEmpty();
    
    // Returns how many events are waiting
    int size();
};

#endif