#!/bin/bash

# ====================================================
# PROJECT 2: PRE-COG SCHEDULER (FULL PIPELINE)
# ====================================================

# Colors for nice output
GREEN='\033[0;32m'
CYAN='\033[0;36m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${CYAN}[*] Initializing Pre-Cog System Pipeline...${NC}"

# 1. PYTHON ENVIRONMENT CHECK
# ---------------------------
if [ ! -d "venv" ]; then
    echo -e "${RED}[!] No virtual environment found. Creating one...${NC}"
    python3 -m venv venv
fi
source venv/bin/activate
# Install matplotlib if missing (suppress output)
if ! python3 -c "import matplotlib" 2>/dev/null; then
    echo -e "${CYAN}[*] Installing Matplotlib for visualization...${NC}"
    pip install matplotlib > /dev/null 2>&1
fi

# 2. DATA GENERATION
# ------------------
echo -e "${GREEN}[1/4] Generating Synthetic Workload (1,000 Processes)...${NC}"
python3 generate_workload.py
if [ ! -f "tasks.csv" ]; then
    echo -e "${RED}[ERROR] Data generation failed!${NC}"
    exit 1
fi

# 3. COMPILATION
# --------------
echo -e "${GREEN}[2/4] Compiling C++ Core Engine...${NC}"
g++ src/main_extensive.cpp -o huge_sim
if [ $? -ne 0 ]; then
    echo -e "${RED}[ERROR] Compilation failed!${NC}"
    exit 1
fi

# 4. EXECUTION (THE RACE)
# -----------------------
echo -e "${GREEN}[3/4] Running Simulation Race (FIFO vs Pre-Cog AI)...${NC}"
echo "--------------------------------------------------------"
./huge_sim
echo "--------------------------------------------------------"

# 5. VISUALIZATION
# ----------------
echo -e "${GREEN}[4/4] Generating Performance Chart...${NC}"
python3 visualize_results.py

echo -e "${CYAN}========================================================${NC}"
echo -e "${CYAN}[SUCCESS] Pipeline Complete.${NC}"
echo -e "          1. Raw Data:    tasks.csv"
echo -e "          2. Visual Proof: comparison_chart.png"
echo -e "${CYAN}========================================================${NC}"