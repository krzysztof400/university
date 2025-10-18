#!/bin/bash

tests=("r9" "r10" "r12" "r18" "r20" "r28" "r36" "r40" "r42" "r56" "r59")
for i in "${!tests[@]}"; do
  echo "Running test ${tests[$i]}..."
  echo ""
  source ./"${tests[$i]}.sh"
  echo ""
done