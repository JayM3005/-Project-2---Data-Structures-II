#include <iostream>
#include <fstream>
#include <cmath>    
#include <cstdlib>  
#include <ctime>
#include <iomanip> 
#include "PriorityQueue.hpp"
#include "FIFOQueue.hpp"

using namespace std;

// Math Helper Function
double factorial(int n) {
    if (n <= 1) return 1.0;
    double fact = 1.0;
    for (int i = 2; i <= n; ++i) {
        fact *= i;
    }
    return fact;
}

double GetNextRandomInterval(double avg) {
    double f = (double)rand() / RAND_MAX;
    while (f == 0.0) { 
        f = (double)rand() / RAND_MAX;
    }
    return -1.0 * (1.0 / avg) * log(f); 
}

// Wrapper function to run the simulation for any given text file
void runSimulation(string filename) {
    ifstream infile(filename);
    if (!infile) {
        cout << "Error opening file: " << filename << endl;
        return; 
    }

    double lambda, mu;       
    int M, totalEvents; 
    infile >> lambda >> mu >> M >> totalEvents;
    infile.close();

    cout << "\n==================================================" << endl;
    cout << "Running Simulation for: " << filename << endl;
    cout << "Lambda: " << lambda << " | Mu: " << mu << " | Servers: " << M << " | Events: " << totalEvents << endl;
    cout << "==================================================" << endl;

    // Simulation Engine
    double currentTime = 0.0;
    int availableServers = M;
    PriorityQueue pq;
    FIFOQueue fifo;
    int eventsProcessed = 0; 
    
    // Batching Logic
    double lastArrivalTime = 0.0; 
    int maxArrivals = totalEvents / 2; 
    int arrivalsGenerated = 0;

    // Tracking Variables
    double totalServiceTime = 0.0;
    double totalWaitTime = 0.0;
    int totalCustomersWaited = 0;
    int totalCustomersServed = 0;
    
    double totalIdleTime = 0.0;
    double idleStartTime = 0.0; 
    bool systemIsIdle = true;

    // 1. Kickstarts the first batch of arrivals
    while (pq.size() < 200 && arrivalsGenerated < maxArrivals) {
        lastArrivalTime += GetNextRandomInterval(lambda);
        Event newArrival;
        newArrival.time = lastArrivalTime;
        newArrival.type = 0; 
        pq.insert(newArrival);
        arrivalsGenerated++;
    }

    // Event Loop
    while (!pq.isEmpty() && eventsProcessed < totalEvents) {
        
        Event currentEvent = pq.deleteMin();
        currentTime = currentEvent.time; 
        
        if (currentEvent.type == 0) {
            // Arrival Logic
            
            // Tracking - If the system was idle, changes to not being idle and records the duration. 
            if (systemIsIdle) {
                totalIdleTime += (currentTime - idleStartTime);
                systemIsIdle = false;
            }

            if (availableServers > 0) {
                availableServers--;
                
                // Tracking - Calculates service duration for the service
                double serviceDuration = GetNextRandomInterval(mu);
                totalServiceTime += serviceDuration;
                totalCustomersServed++;

                Event departure;
                departure.time = currentTime + serviceDuration;
                departure.type = 1; 
                pq.insert(departure);
            } else {
                fifo.enqueue(currentTime);
            }
            
        } else if (currentEvent.type == 1) {
            // Departure Logic
            availableServers++; 
            
            if (!fifo.isEmpty()) {
                double customerArrivalTime = fifo.dequeue();
                availableServers--;
                
                // Tracking - Calculates wait time for those in line. 
                double waitDuration = currentTime - customerArrivalTime;
                totalWaitTime += waitDuration;
                totalCustomersWaited++;

                // Tracking - Calculates service duration
                double serviceDuration = GetNextRandomInterval(mu);
                totalServiceTime += serviceDuration;
                totalCustomersServed++;
                
                Event departure;
                departure.time = currentTime + serviceDuration;
                departure.type = 1;
                pq.insert(departure);
            }

            // Tracking - If all servers are free and line is empty, the idle clock starts.
            if (availableServers == M && fifo.isEmpty()) {
                systemIsIdle = true;
                idleStartTime = currentTime;
            }
        }
        
        eventsProcessed++;
        
        // Priority Queue Refilling Logic
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

    // Calculates the Analytical Model 
    double sum = 0.0;
    for (int i = 0; i < M; ++i) {
        sum += (1.0 / factorial(i)) * pow(lambda / mu, i);
    }
    double part2 = (1.0 / factorial(M)) * pow(lambda / mu, M) * ((M * mu) / (M * mu - lambda));
    
    double analytical_PO = 1.0 / (sum + part2);
    double analytical_L = ((lambda * mu * pow(lambda / mu, M)) / (factorial(M - 1) * pow(M * mu - lambda, 2))) * analytical_PO + (lambda / mu);
    double analytical_W = analytical_L / lambda;
    double analytical_LQ = analytical_L - (lambda / mu);
    double analytical_WQ = analytical_LQ / lambda;
    double analytical_RHO = lambda / (M * mu);

    // Calculates the measured simulation
    double sim_Po = totalIdleTime / currentTime;
    double sim_WQ = totalWaitTime / totalCustomersServed;
    double sim_W = sim_WQ + (totalServiceTime / totalCustomersServed);
    double sim_RHO = totalServiceTime / (M * currentTime);
    double probWait = (double)totalCustomersWaited / totalCustomersServed;

    // Prints the comparison
    cout << fixed << setprecision(4);
    cout << left << setw(30) << "Measure" << setw(20) << "Analytical" << setw(20) << "Simulation" << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << left << setw(30) << "Po (Percent Idle Time)" << setw(20) << analytical_PO << setw(20) << sim_Po << endl;
    cout << left << setw(30) << "W  (Time in System)" << setw(20) << analytical_W << setw(20) << sim_W << endl;
    cout << left << setw(30) << "Wq (Wait Time)" << setw(20) << analytical_WQ << setw(20) << sim_WQ << endl;
    cout << left << setw(30) << "Rho(Utilization Factor)" << setw(20) << analytical_RHO << setw(20) << sim_RHO << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "Probability of Waiting: " << probWait << "\n" << endl;
}

int main() {
    srand(time(0));
    runSimulation("test1.txt");
    runSimulation("test2.txt");
    return 0;
}