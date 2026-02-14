#include "PriorityQueue.hpp"

// Constructor
PriorityQueue::PriorityQueue() {
    Event bot; 
    bot.time = -1.0; 
    heap.push_back(bot);
}

bool PriorityQueue::isEmpty() {
    return heap.size() <= 1;
}

int PriorityQueue::size() {
    return heap.size() - 1;
}

void PriorityQueue::insert(Event e) {
    // 1 - Adds to the very end
    heap.push_back(e);
    
    // 2 - Fixes the order (swim up)
    int index = heap.size() - 1;
    percolateUp(index);
}

Event PriorityQueue::deleteMin() {
    if (isEmpty()) {
        // Returns a dummy error event if empty
        Event e; e.time = -1; return e; 
    }

    Event minEvent = heap[1];

    // 1 - Moves the very last element to the top
    heap[1] = heap.back();
    heap.pop_back();

    // 2 Fixes the order (sink down)
    if (!isEmpty()) {
        percolateDown(1);
    }

    return minEvent;
}

// Fixes order from bottom to the top
void PriorityQueue::percolateUp(int index) {
    while (index > 1) {
        int parent = index / 2;
        
        if (heap[index].time < heap[parent].time) {
            swap(heap[index], heap[parent]);
            index = parent; 
        } else {
            break; 
        }
    }
}

// Fixes order from top to bottom
void PriorityQueue::percolateDown(int index) {
    int child;
    int size = heap.size() - 1;

    while (index * 2 <= size) {
        child = index * 2; 
        
        // Checks if RIGHT child exists AND is smaller than LEFT
        if (child != size && heap[child + 1].time < heap[child].time) {
            child++; 
        }

        if (heap[child].time < heap[index].time) {
            swap(heap[child], heap[index]);
            index = child; 
        } else {
            break; 
        }
    }
}