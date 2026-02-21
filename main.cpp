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

// Wrapper function to run the simulation for any given text file
void runSimulation(string filename) {
    // 1. Reads the input file
    ifstream infile(filename);

    if (!infile) {
        cout << "Error opening file: " << filename << endl;
        return; 
    }

    double lambda;   
    double mu;       
    int M;           
    int totalEvents; 

    infile >> lambda >> mu >> M >> totalEvents;
    infile.close();

    // Checks if reading correctly 
    cout << "\n--- Simulation Parameters (" << filename << ") ---" << endl;
    cout << "Lambda (Arrival Rate) : " << lambda << endl;
    cout << "Mu (Service Rate)     : " << mu << endl;
    cout << "M (Servers)           : " << M << endl;
    cout << "Total Events          : " << totalEvents << endl;
    cout << "---------------------------------------" << endl;

    // Simulation Engine Setup
    double currentTime = 0.0;
    int availableServers = M;
    PriorityQueue pq;
    FIFOQueue fifo;
    
    int eventsProcessed = 0;
    
    // Batching Logic Setup
    double lastArrivalTime = 0.0; 
    int maxArrivals = totalEvents / 2; 
    int arrivalsGenerated = 0;

    // 1. Starts the simulation with the first batch of arrivals
    while (pq.size() < 200 && arrivalsGenerated < maxArrivals) {
        lastArrivalTime += GetNextRandomInterval(lambda);
        Event newArrival;
        newArrival.time = lastArrivalTime;
        newArrival.type = 0; 
        pq.insert(newArrival);
        arrivalsGenerated++;
    }

    cout << "Starting simulation..." << endl;

    // Event Loop
    while (!pq.isEmpty() && eventsProcessed < totalEvents) {
        
        // Skips to the next event
        Event currentEvent = pq.deleteMin();
        currentTime = currentEvent.time; 
        
        if (currentEvent.type == 0) {
            // Arrival Logic
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
        
        // Priority Queue Refill Logic
        // Keeps the priority queue small. Refills when it drops to M+1
        if (pq.size() == M + 1 && arrivalsGenerated < maxArrivals) {
            while (pq.size() < 200 && arrivalsGenerated < maxArrivals) {
                lastArrivalTime += GetNextRandomInterval(lambda);
                Event newArrival;
                newArrival.time = lastArrivalTime;
                newArrival.type = 0;
                pq.insert(newArrival);
                arrivalsGenerated++;
            }
        }
    }

    cout << "Simulation complete!" << endl;
    cout << "Total Events Processed: " << eventsProcessed << endl;
    cout << "Final Simulation Time : " << currentTime << endl;
}

int main() {
    // Seeds the random number generator 
    srand(time(0));

    // Automates running both simulation files
    runSimulation("test1.txt");
    runSimulation("test2.txt");

    return 0;
}