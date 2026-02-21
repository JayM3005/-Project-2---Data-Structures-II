#include <iostream>
#include <fstream>
#include <cmath>    
#include <cstdlib>  
#include <ctime>    
#include "PriorityQueue.hpp"
#include "FIFOQueue.hpp"

using namespace std;

// Algorithm 
double GetNextRandomInterval(double avg) {
    // 1. Generate a random float 'f' between 0 and 1
    double f = (double)rand() / RAND_MAX;
    
    while (f == 0.0) { 
        f = (double)rand() / RAND_MAX;
    }
    
    // 2. Compute interval time: -1 * (1.0/avg) * ln(f)
    double intervalTime = -1.0 * (1.0 / avg) * log(f); 
    
    return intervalTime;
}

int main() {
    // Seeds the random number generator 
    srand(time(0));

    // 1. Read the input file
    string filename = "test1.txt"; 
    ifstream infile(filename);

    if (!infile) {
        cout << "Error opening file: " << filename << endl;
        return 1; 
    }

    double lambda;   
    double mu;       
    int M;           
    int totalEvents; 

    infile >> lambda >> mu >> M >> totalEvents;
    infile.close();

    // Just printing to verify we read it correctly
    cout << "--- Simulation Parameters ---" << endl;
    cout << "Lambda (Arrival Rate) : " << lambda << endl;
    cout << "Mu (Service Rate)     : " << mu << endl;
    cout << "M (Servers)           : " << M << endl;
    cout << "Total Events          : " << totalEvents << endl;
    cout << "-----------------------------" << endl;

    // --- SIMULATION ENGINE SETUP ---
    double currentTime = 0.0;
    int availableServers = M;
    PriorityQueue pq;
    FIFOQueue fifo;
    
    int eventsProcessed = 0; // To keep track of our 5000 limit

    // 1. Kickstart the simulation with the very first arrival
    Event firstArrival;
    firstArrival.time = GetNextRandomInterval(lambda);
    firstArrival.type = 0; // 0 = Arrival
    pq.insert(firstArrival);

    cout << "Starting simulation..." << endl;

    // Event Loop
    while (!pq.isEmpty() && eventsProcessed < totalEvents) {
        
        // Skips to the next event
        Event currentEvent = pq.deleteMin();
        currentTime = currentEvent.time; 
        
        if (currentEvent.type == 0) {
            // Arrival Logic
            
            // 1. Schedules the next person to arrive (if we haven't hit the limit)
            if (eventsProcessed < totalEvents) {
                Event nextArrival;
                nextArrival.time = currentTime + GetNextRandomInterval(lambda);
                nextArrival.type = 0;
                pq.insert(nextArrival);
            }

            // 2. Processes the person
            if (availableServers > 0) {
                // If server is free, take them immediately.
                availableServers--;
                
                // Figures out when they will leave
                Event departure;
                departure.time = currentTime + GetNextRandomInterval(mu);
                departure.type = 1; 
                pq.insert(departure);
            } else {
                // If servers are full, goes to wait in line.
                fifo.enqueue(currentTime);
            }
            
        } else if (currentEvent.type == 1) {
            // Departure Logic
            
            availableServers++; 
            
            // Checks if anyone is waiting in the FIFO line
            if (!fifo.isEmpty()) {
                // Grabs the next person in line
                double customerArrivalTime = fifo.dequeue();
                
                // Server immediately takes them
                availableServers--;
                
                // Schedules when the person will leave
                Event departure;
                departure.time = currentTime + GetNextRandomInterval(mu);
                departure.type = 1;
                pq.insert(departure);
            }
        }
        
        eventsProcessed++;
    }

    cout << "Simulation complete!" << endl;
    cout << "Total Events Processed: " << eventsProcessed << endl;
    cout << "Final Simulation Time : " << currentTime << endl;

    return 0;
}