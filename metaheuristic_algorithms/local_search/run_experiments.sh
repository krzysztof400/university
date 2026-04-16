#!/bin/bash
set -e

# Configuration
SOURCE_FILE="graph.cpp"
EXEC_FILE="./tsp_solver"
RESULTS_DIR="./batch_results"

# Check for folder argument
if [ -z "$1" ]; then
    echo "Usage: ./run_batch.sh <path_to_folder_with_stp_files>"
    exit 1
fi
DATA_FOLDER="$1"

# 1. Compile once
echo "Compiling $SOURCE_FILE..."
g++ -O3 -march=native -pthread -std=c++17 -o "$EXEC_FILE" "$SOURCE_FILE"

# 2. Create a dedicated folder for the output logs
mkdir -p "$RESULTS_DIR"

# 3. Loop through every .stp file in the target folder
# (If your files are .tsp, change the extension in the loop below)
for file in "$DATA_FOLDER"/*.tsp; do
    # Skip if no files match
    [ -e "$file" ] || continue 
    
    # Extract just the filename (e.g., "graph1.stp") for the log name
    filename=$(basename "$file")
    log_file="$RESULTS_DIR/${filename}.log"
    
    echo "========================================"
    echo "Processing: $filename"
    echo "Saving to:  $log_file"
    echo "========================================"
    
    # Run the solver on this specific file, routing output to its own log
    stdbuf -o0 "$EXEC_FILE" "$file" 2>&1 | tee "$log_file"
    
    # Force disk write in case of a crash before the next file starts
    sync
done

echo "Batch complete! All logs saved in $RESULTS_DIR/"