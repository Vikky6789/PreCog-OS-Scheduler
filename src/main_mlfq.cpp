#include <iostream>
#include <vector>
#include "process.h"
#include "scheduler_mlfq.h"

int main() {
    MLFQScheduler os;

    // WORKLOAD SETUP
    // 1. "Mouse_Driver": Very fast, needs instant reaction (5ms)
    // 2. "Background_Backup": Huge, slow (200ms)
    // 3. "Firefox": Medium, spiky (40ms)
    
    // Note: We initialize them with 'burst_time'. 
    // In a real loop, the AI would learn these over time. 
    // For this demo, we pre-seed the 'prediction' to simulate a learned state.
    
    Process p1(1, "Mouse_Driver", 0, 5);
    p1.predicted_next_burst = 5; // AI knows it's fast

    Process p2(2, "Backup_Service", 0, 200);
    p2.predicted_next_burst = 200; // AI knows it's huge

    Process p3(3, "Firefox_Tab", 0, 40);
    p3.predicted_next_burst = 40; // AI knows it's medium

    std::cout << "=== STARTING AI-ACCELERATED MLFQ KERNEL ===\n";
    
    // Add them all at once (Time 0)
    os.add_process(&p1);
    os.add_process(&p2);
    os.add_process(&p3);

    std::cout << "\n[KERNEL] Starting execution loop...\n";

    // Run until empty
    while (!os.is_idle()) {
        os.run_tick();
    }

    std::cout << "=== SYSTEM HALTED ===\n";
    return 0;
}