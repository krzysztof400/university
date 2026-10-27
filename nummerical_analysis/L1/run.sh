#!/usr/bin/env bash
# Runs all tasks and stores the console output in ./output/
# Usage: ./run.sh
set -u
cd "$(dirname "$0")"
mkdir -p output

for i in 1 2 3 4 5 6; do
    echo ">>> zad${i}.jl"
    julia "zad${i}.jl" 2>&1 | tee "output/zad${i}.txt"
    echo
done

cat output/zad*.txt > output/all.txt
echo "Done. Results in output/ (all.txt = combined)."
