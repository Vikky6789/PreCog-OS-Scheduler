#ifndef PROCESS_H
#define PROCESS_H

#include <vector>
#include <iostream>
#include <cmath>

// Process State Enum (Standard OS States)
enum State {
    READY,
    RUNNING,
    WAITING,
    TERMINATED
};

struct Process {
    int pid;                    // Process ID
    std::string name;           // e.g., "Chrome", "VSCode"

    int finish_time = 0;
    int waiting_time = 0;
    int turnaround_time = 0;


    // Scheduling Metrics
    int arrival_time;           // When it was created
    int burst_time;             // How much CPU it *actually* needs (Ground Truth)
    int remaining_time;         // How much is left to finish
    
    // THE AI COMPONENT: Prediction Metrics
    float predicted_next_burst; // What we *think* it needs
    std::vector<int> burst_history; // Memory of past behavior
    
 
    
    int priority_level;  // 0 = High (Interactive), 1 = Med, 2 = Low (Batch)
    int time_slice_used; // How much of the current quantum have we used?

    //  Constructor 
    Process(int id, std::string n, int arrival, int burst) 
        : pid(id), name(n), arrival_time(arrival), burst_time(burst), 
          remaining_time(burst), predicted_next_burst(0), state(READY),
          priority_level(0), time_slice_used(0) {} // Default to Queue 0

    State state;

    // Helper to print status
    void print_info() {
        std::cout << "[PID: " << pid << "] " << name 
                  << " | Burst Needed: " << burst_time 
                  << " | Predicted: " << (int)predicted_next_burst 
                  << " | State: " << state << std::endl;
    }
};

#endif