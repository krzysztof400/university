#!/bin/bash

# Define kernel configurations
r9_configurations=("dmesg_restrict" "kptr_restrict" "pid_max" "perf_cpu_time_max_percent" "perf_event_max_sample_rate" "perf_event_paranoid" "randomize_va_space" "sysrq" "unprivileged_bpf_disabled" "panic_on_oops")
# Define prefered values
r9_expected_vals=(1 2 1000 1 1 2 2 0 1 1)
r9_outputs=()
config_name="kernel"

# Check if the kernel configurations are set to the prefered values
for i in "${!r9_configurations[@]}"; do
  config="${r9_configurations[$i]}"
  actual_value=$(sysctl -n "$config_name.$config" 2>/dev/null)
  expected_value="${r9_expected_vals[$i]}"
  if [[ "$actual_value" == "$expected_value" ]]; then
    echo "OK: $config_name.$config is $actual_value"
    r9_outputs+=("$0")
  else
    # this has to be here because the value of pid_max is not fixed
    if [[ "$config" == "pid_max" ]]; then
      echo "OK: $config_name.$config is $actual_value (value has to be only specified)"
      r9_outputs+=("$0")
    else
      echo "KO: $config_name.$config is $actual_value (expected $expected_value)"
      r9_outputs+=("$1")
    fi
  fi
done