#!/bin/bash

pid_max=$(sysctl -n kernel.pid_max)

recommended_options=(
    "kernel.dmesg_restrict"
    "kernel.kptr_restrict"
    "kernel.pid_max"
    "kernel.perf_cpu_time_max_percent"
    "kernel.perf_event_max_sample_rate"
    "kernel.perf_event_paranoid"
    "kernel.randomize_va_space"
    "kernel.sysrq"
    "kernel.unprivileged_bpf_disabled"
    "kernel.panic_on_oops"
)

for option in "${recommended_options[@]}"; do
    if [ "$option" == "kernel.pid_max" ]; then
        echo "kernel.pid_max = $pid_max"
        continue
    fi 
    sysctl $option
done