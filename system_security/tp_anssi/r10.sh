#!/bin/bash

# Get the value of kernel.modules_disabled
actual_value=$(sysctl -n "kernel.modules_disabled" 2>/dev/null)
# Define the expected value
expected_value=1
# Check if the kernel.modules_disabled is set to the expected value
if [[ "$actual_value" == "$expected_value" ]]; then
  echo "OK: kernel.modules_disabled is $actual_value"
else
  echo "KO: kernel.modules_disabled is $actual_value (expected $expected_value)"
fi
