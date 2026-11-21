#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "${BASH_SOURCE[0]}")"
mkdir -p output
for f in zad[0-9]*.jl; do
    julia "$f" 2>&1 | tee "output/${f%.jl}.txt"
done
cat output/zad[0-9]*.txt > output/all.txt