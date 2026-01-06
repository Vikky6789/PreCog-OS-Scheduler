#ifndef SCHEDULER_MLFQ_H
#define SCHEDULER_MLFQ_H

#include <vector>
#include <deque>
#include <iostream>
#include "process.h"
#include "predictor.h"

class MLFQScheduler {
private:
    // 3 Queues: High (0), Med (1), Low (2)
    std::deque<Process*> q0; 
    std::deque<Process*> q1;
    std::deque<Process*> q2;
    
    CPUPredictor predictor;
    int current_time;

    // Configuration: Time Slices for Q0 and Q1
    const int QUANTUM_Q0 = 10;
    const int QUANTUM_Q1 = 50;

public:
    MLFQScheduler() : current_time(0), predictor(0.5) {}

    // 1. INTELLIGENT ADMISSION (The "Pre-Cog" Upgrade)
    void add_process(Process* p) {
        // PREDICTION:
        // If the AI thinks this job will be > 60ms (Q0+Q1 capacity), 
        // skip the line and banish it to Q2 immediately.
        // This keeps Q0 empty for REAL mouse clicks/keypresses.
        
        if (p->predicted_next_burst > (QUANTUM_Q0 + QUANTUM_Q1)) {
            p->priority_level = 2; // Direct to Low Priority
            q2.push_back(p);
            std::cout << "[AI-OS] Process " << p->name << " (Pred: " << p->predicted_next_burst 
                      << "ms) classified as HEAVY. Sent to Queue 2.\n";
        } 
        else if (p->predicted_next_burst > QUANTUM_Q0) {
            p->priority_level = 1; // Direct to Mid Priority
            q1.push_back(p);
            std::cout << "[AI-OS] Process " << p->name << " classified as MEDIUM. Sent to Queue 1.\n";
        }
        else {
            p->priority_level = 0; // Standard High Priority
            q0.push_back(p);
            std::cout << "[AI-OS] Process " << p->name << " classified as INTERACTIVE. Sent to Queue 0.\n";
        }
    }

    // 2. THE RUNNER
    void run_tick() {
        Process* current = nullptr;
        int quantum = 0;

        // PICK PROCESS (Priority: Q0 > Q1 > Q2)
        if (!q0.empty())      { current = q0.front(); quantum = QUANTUM_Q0; }
        else if (!q1.empty()) { current = q1.front(); quantum = QUANTUM_Q1; }
        else if (!q2.empty()) { current = q2.front(); quantum = 999999; } // FCFS
        else { 
            current_time++; // IDLE
            return; 
        }

        // SIMULATE 1 MILLISECOND OF EXECUTION
        current->remaining_time--;
        current->time_slice_used++;
        current_time++;

        // CHECK STATUS
        
        // A. FINISHED?
        if (current->remaining_time <= 0) {
            std::cout << "[TIME " << current_time << "] FINISHED: " << current->name 
                      << " (Level " << current->priority_level << ")\n";
            
            // AI FEEDBACK LOOP: Update prediction for next time
            predictor.update_prediction(current, current->burst_time);
            
            // Remove from queue
            pop_current_queue(current->priority_level);
        }
        // B. TIME SLICE EXPIRED? (Demotion)
        else if (current->time_slice_used >= quantum) {
            // Punish the process! Move it down a level.
            int old_level = current->priority_level;
            pop_current_queue(old_level);
            
            current->time_slice_used = 0; // Reset quantum tracker
            
            if (current->priority_level < 2) {
                current->priority_level++; // Demote (0->1 or 1->2)
            }
            
            // Add to back of new queue
            if (current->priority_level == 1) q1.push_back(current);
            else q2.push_back(current);

            std::cout << "[TIME " << current_time << "] DEMOTED: " << current->name 
                      << " (" << old_level << " -> " << current->priority_level << ")\n";
        }
    }

    void pop_current_queue(int level) {
        if (level == 0) q0.pop_front();
        else if (level == 1) q1.pop_front();
        else q2.pop_front();
    }
    
    bool is_idle() {
        return q0.empty() && q1.empty() && q2.empty();
    }
};

#endif