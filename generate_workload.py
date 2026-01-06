import random
import csv

# Configuration
NUM_PROCESSES = 1000
FILENAME = "tasks.csv"

print(f"[*] Generating {NUM_PROCESSES} complex process traces...")

with open(FILENAME, 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    # Header: PID, Name, Arrival_Time, Burst_Time
    writer.writerow(["PID", "Type", "Arrival", "Burst"])

    current_time = 0
    
    for i in range(1, NUM_PROCESSES + 1):
        # 30% are Heavy Tasks (Video Encode, Compile) -> Mean 50ms, StdDev 10ms
        # 70% are Light Tasks (UI, Network)         -> Mean 5ms,  StdDev 2ms
        
        if random.random() < 0.3:
            p_type = "HEAVY_CPU"
            burst = int(random.gauss(50, 10))
        else:
            p_type = "LIGHT_IO"
            burst = int(random.gauss(5, 2))

        # Ensure burst is at least 1ms
        burst = max(1, burst)
        
        # Processes arrive randomly (Poisson process simulation)
        # Sometimes 0ms gap (simultaneous), sometimes 5ms gap
        arrival_gap = int(random.expovariate(0.5)) 
        current_time += arrival_gap

        writer.writerow([i, p_type, current_time, burst])

print(f"[*] Done. Saved to {FILENAME}")