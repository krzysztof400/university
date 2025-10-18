#!/bin/bash

# Define complile options for kernel modules
r18_configurations=("CONFIG_MODULES" "CONFIG_STRICT_MODULE_RWX" "CONFIG_MODULE_SIG" "CONFIG_MODULE_SIG_FORCE"
"CONFIG_MODULE_SIG_ALL" "CONFIG_MODULE_SIG_SHA512" "CONFIG_MODULE_SIG_HASH" "CONFIG_MODULE_SIG_KEY")
# Define prefered values
r18_expected_vals=("y" "y" "y" "y" "y" "y" "\"sha512\"" "\"certs/signing_key.pem\"")
# used for if you only want to print a single OK or KO
r18_outputs=()

# Check if the complile options are set to the prefered values
for i in "${!r18_configurations[@]}"; do
  config="${r18_configurations[$i]}"
  actual_value=$(zcat /proc/config.gz | grep -w "$config" 2>/dev/null)
  expected_value="$config=${r18_expected_vals[$i]}"
  if [[ "$actual_value" == "$expected_value" ]]; then
    echo "OK: $config is $actual_value"
    r18_outputs+=("$0")
  else
    echo "KO: $config is $actual_value (expected $expected_value)"
    r18_outputs+=("$1")
  fi
done