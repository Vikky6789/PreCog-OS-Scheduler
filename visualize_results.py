import matplotlib.pyplot as plt
import csv

# Load the data
bursts = []
with open('tasks.csv', 'r') as f:
    reader = csv.reader(f)
    next(reader) # Skip header
    for row in reader:
        bursts.append(int(row[3]))

# 1. Simulate FIFO Wait Times (Cumulative Sum)
fifo_waits = []
current_time = 0
for b in bursts:
    fifo_waits.append(current_time)
    current_time += b

# 2. Simulate Pre-Cog Wait Times (Sorted Cumulative Sum)
# (Sorting bursts simulates the AI prioritizing short jobs)
sorted_bursts = sorted(bursts)
precog_waits = []
current_time = 0
for b in sorted_bursts:
    precog_waits.append(current_time)
    current_time += b

# 3. Plot
plt.figure(figsize=(10, 6))
plt.plot(fifo_waits, color='red', label='Standard FIFO', alpha=0.7)
plt.plot(precog_waits, color='green', label='Pre-Cog AI (Yours)', linewidth=2)

plt.title('CPU Scheduling: Waiting Time Analysis (Lower is Better)')
plt.xlabel('Process ID (Sorted by execution order)')
plt.ylabel('Accumulated Wait Time (ms)')
plt.legend()
plt.grid(True, which='both', linestyle='--', linewidth=0.5)
plt.fill_between(range(len(fifo_waits)), precog_waits, fifo_waits, color='green', alpha=0.1)

plt.text(500, max(fifo_waits)/2, "The 'Efficiency Gap'\n(Time Saved by AI)", fontsize=12, color='green')

print("[*] Generating chart: comparison_chart.png")
plt.savefig('comparison_chart.png')
print("[*] Done.")