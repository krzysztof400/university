#!/bin/bash

# Define kernel complie options for security functions
r20_configurations=("CONFIG_SECCOMP" "CONFIG_SECCOMP_FILTER" "CONFIG_SECURITY" "CONFIG_SECURITY_YAMA"
"CONFIG_SECURITY_WRITABLE_HOOKS")
# Define prefered values
r20_expected_vals=("y" "y" "y" "y" " is not set")
# used for if you only want to print a single OK or KO
r20_outputs=()

# Check if the kernel configurations are set to the prefered values
for i in "${!r20_configurations[@]}"; do
  config="${r20_configurations[$i]}"
  actual_value=$(zcat /proc/config.gz | grep -w "$config" 2>/dev/null)
  expected_value="$config=${r20_expected_vals[$i]}"
  if [[ "$actual_value" == "$expected_value" ]]; then
    echo "OK: $config is $actual_value"
    r20_outputs+=("$0")
  else
    # check if not set message for specific config
    if [[ "$config" == "CONFIG_SECURITY_WRITABLE_HOOKS" ]]; then
      if [[ $actual_value == "# CONFIG_SECURITY_WRITABLE_HOOKS is not set" || $actual_value == "" ]]; then
        echo "OK: $config is $actual_value"
        r20_outputs+=("$0")
      else
        echo "KO: $config is $actual_value (expected $config is not set)"
        r20_outputs+=("$1")
      fi
    else
      echo "KO: $config is $actual_value (expected $expected_value)"
      r20_outputs+=("$1")
    fi
  fi
done